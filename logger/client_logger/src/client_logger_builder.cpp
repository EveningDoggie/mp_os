#include <not_implemented.h>

#include "../include/client_logger_builder.h"
#include <stdlib.h>

client_logger* _client_logger;

client_logger_builder::client_logger_builder()
{
    _client_logger = new client_logger();
}

client_logger_builder::client_logger_builder(
    client_logger_builder const &other)
{
    throw not_implemented("client_logger_builder::client_logger_builder(client_logger_builder const &other)", "your code should be here...");
}

client_logger_builder &client_logger_builder::operator=(
    client_logger_builder const &other)
{
    throw not_implemented("client_logger_builder &client_logger_builder::operator=(client_logger_builder const &other)", "your code should be here...");
}

client_logger_builder::client_logger_builder(
    client_logger_builder &&other) noexcept
{
    throw not_implemented("client_logger_builder::client_logger_builder(client_logger_builder &&other) noexcept", "your code should be here...");
}

client_logger_builder &client_logger_builder::operator=(
    client_logger_builder &&other) noexcept
{
    throw not_implemented("client_logger_builder &client_logger_builder::operator=(client_logger_builder &&other) noexcept", "your code should be here...");
}

client_logger_builder::~client_logger_builder() noexcept
{
    if (_client_logger != NULL)
    {
       // clear();
       // delete _client_logger;
    }
}

logger_builder* client_logger_builder::add_file_stream(
    std::string const& stream_file_path,
    std::set<logger::severity> severitys)
{
    std::string absolute_path = get_file_absolute_path(stream_file_path);
    auto files_streams_all_ptr = &_client_logger->_files_streams_all;
    auto files_streams_local_ptr = &_client_logger->_files_streams_local;
    auto file_all_not_yet_contained = files_streams_all_ptr->find(absolute_path) == files_streams_all_ptr->end();
    auto file_not_yet_contained = files_streams_local_ptr->find(absolute_path) == files_streams_local_ptr->end();
    auto files_streams_all_pair_ptr = &((*files_streams_all_ptr)[absolute_path]);
    auto files_streams_pair_ptr = &((*files_streams_local_ptr)[absolute_path]);

    if (file_all_not_yet_contained)
    {
        files_streams_all_pair_ptr->first = new std::ofstream(absolute_path, std::ios::app);
        files_streams_all_pair_ptr->second = 0;
        if (files_streams_all_pair_ptr->first->is_open() == false)
        {
            delete files_streams_all_pair_ptr->first;
            throw "File not found at this path";
        }
    }

    if (file_not_yet_contained)
    {
        files_streams_all_pair_ptr->second++;
        files_streams_pair_ptr->first = files_streams_all_pair_ptr->first;
        _saved_patches.insert(absolute_path);
    }
    files_streams_pair_ptr->second.insert(severitys.begin(), severitys.end());

}


logger_builder *client_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    add_file_stream(stream_file_path, get_set_from_minimal_severity(severity));
    return this;
  }

std::set<logger::severity> client_logger_builder::get_set_from_minimal_severity(logger::severity severity)
{
    std::set<logger::severity> severitys;
    switch (severity)
    {
        case logger::severity::trace: severitys.insert(logger::severity::trace);
        case logger::severity::debug: severitys.insert(logger::severity::debug);
        case logger::severity::information: severitys.insert(logger::severity::information);
        case logger::severity::warning: severitys.insert(logger::severity::warning);
        case logger::severity::error: severitys.insert(logger::severity::error);
        case logger::severity::critical: severitys.insert(logger::severity::critical);    
    }
    return severitys;
}


logger_builder *client_logger_builder::add_console_stream(
    logger::severity severity)
{
    add_console_stream(get_set_from_minimal_severity(severity));
    return this;
}

logger_builder* client_logger_builder::add_console_stream(
    std::set<logger::severity> severitys)
{
    _client_logger->_console_streams_local.insert(severitys.begin(), severitys.end());
    return this;
}


logger_builder *client_logger_builder::clear()
{
    _client_logger->_console_streams_local .clear();
    _client_logger->_files_streams_local.clear();

    for (auto path : _saved_patches)
    {
        if (_client_logger->_files_streams_all.find(path) != _client_logger->_files_streams_all.end())
        {
            std::ofstream* out = _client_logger->_files_streams_all[path].first;
            out->close();
            delete out;

            _client_logger->_files_streams_all.erase(path);
        }
    }

    _saved_patches.clear();
    return this;
}

logger *client_logger_builder::build() const
{
    auto ref = _client_logger;
    _client_logger = NULL;
    return ref;
}

std::string client_logger_builder::get_file_absolute_path(std::string str)
{
    return std::filesystem::absolute(str).string();
}

logger_builder* client_logger_builder::set_message_mask(
    std::string const& format_mask)
{
    _client_logger->_log_format_mask = format_mask;
    return this;
}


logger_builder* client_logger_builder::transform_with_configuration(
    std::string const& configuration_file_path)
{
    std::ifstream in(configuration_file_path);
    std::string line;
    std::string path;
    logger::severity severity;
    bool last_string_is_path = false;

    if(!in.is_open()) throw "Input configuration file is can't be opened";
    else
    {
        while (std::getline(in, line))
        {
            if (line.find("minimal_severity:") == 0 && last_string_is_path)
            {
                try { severity = string_to_severity(line.erase(0, 17)); }
                catch (const char* error_message) { throw "incorrect data in logger configuration file"; }

                add_file_stream(path, severity);
            }

            if (line.find("mask:") == 0)
                set_message_mask(line.erase(0, 5));
            
            if (line.find("path:") == 0)
            {
                path = line.erase(0, 5);
                last_string_is_path = true;
            }
            else last_string_is_path = false;
        }
    }
    
    in.close();   
    return this;

    //mask: [% s] [% d % t] % m

    //path : file1_config.txt
    //minimal_severity : debug

}
