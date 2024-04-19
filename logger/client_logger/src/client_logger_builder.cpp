#include <not_implemented.h>

#include "../include/client_logger_builder.h"
#include <stdlib.h>


client_logger_builder::client_logger_builder()
{

}

client_logger_builder::client_logger_builder(
    client_logger_builder const &other)
{
    _files_streams = other._files_streams;
    _console_streams = other._console_streams;
    _log_format_mask = other._log_format_mask;
}

client_logger_builder &client_logger_builder::operator=(
    client_logger_builder const &other)
{
    if (&other != this)
    {
        client_logger_builder(other);
    }
    
    return *this;
}

client_logger_builder::client_logger_builder(
    client_logger_builder &&other) noexcept
{
    _files_streams = std::move(other._files_streams);
    _console_streams = std::move(other._console_streams);
    _log_format_mask = std::move(other._log_format_mask);
}

client_logger_builder &client_logger_builder::operator=(
    client_logger_builder &&other) noexcept
{
    if (&other != this)
    {
        client_logger_builder(other);
    }

    return *this;
}

client_logger_builder::~client_logger_builder() noexcept
{

}

logger_builder* client_logger_builder::add_console_stream(
    logger::severity severity)
{
    _console_streams.insert(severity);
    return this;
}

logger_builder* client_logger_builder::add_console_stream_minimal_severity(
    logger::severity severity)
{
    add_console_stream(get_set_from_minimal_severity(severity));
    return this;
}

logger_builder* client_logger_builder::add_console_stream(
    std::set<logger::severity> severitys)
{
    _console_streams.insert(severitys.begin(), severitys.end());
    return this;
}

logger_builder* client_logger_builder::add_file_stream(
    std::string const& stream_file_path,
    logger::severity severity)
{
    _files_streams[get_file_absolute_path(stream_file_path)]
        .insert(severity);
    return this;
}

logger_builder* client_logger_builder::add_file_stream_minimal_severity(
    std::string const& stream_file_path,
    logger::severity severity)
{
    add_file_stream(stream_file_path, get_set_from_minimal_severity(severity));
    return this;
}

logger_builder* client_logger_builder::add_file_stream(
    std::string const& stream_file_path,
    std::set<logger::severity> severitys)
{
    _files_streams[get_file_absolute_path(stream_file_path)]
        .insert(severitys.begin(), severitys.end());

    return this;
}


logger_builder *client_logger_builder::clear()
{
    _console_streams.clear();
    _files_streams.clear();
    _log_format_mask = "[%s][%d %t] %m";
    return this;
}

logger *client_logger_builder::build() const
{
    return new client_logger(_files_streams, _console_streams, _log_format_mask);
}



logger_builder* client_logger_builder::transform_with_configuration(
    std::string const& configuration_file_path)
{
    std::ifstream in(configuration_file_path);
    std::string line;
    std::string path;
    logger::severity severity;
    bool last_string_is_path = false;
    bool last_string_is_console = false;

    if (!in.is_open()) throw "Input configuration file is can't be opened";
    else
    {
        while (std::getline(in, line))
        {

            if (last_string_is_path || last_string_is_console)
            {
                try
                {
                    if (line.find("severity:") == 0)
                    {
                        severity = string_to_severity(line.erase(0, line.find_first_of(":") + 1));
                        if (last_string_is_path) add_file_stream(path, severity);
                        if (last_string_is_console) add_console_stream(severity);
                    }

                    if (line.find("minimal_severity:") == 0)
                    {
                        severity = string_to_severity(line.erase(0, line.find_first_of(":") + 1));
                        if (last_string_is_path) add_file_stream_minimal_severity(path, severity);
                        if (last_string_is_console) add_console_stream_minimal_severity(severity);
                    }

                    continue;
                }
                catch (const char* error_message) { throw "incorrect data in logger configuration file"; }

            }
            
            if (line.find("mask:") == 0)
            {
                change_message_mask(line.erase(0, 5));
            }

            last_string_is_path = line.find("path:") == 0;
            last_string_is_console = line.find("console:") == 0;
        }
    }
    
    in.close();   
    return this;
}


std::string client_logger_builder::get_file_absolute_path(std::string str)
{
    return std::filesystem::absolute(str).string();
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


logger_builder* client_logger_builder::change_message_mask(
    std::string const& mask)
{
    _log_format_mask = mask;
    return this;
}