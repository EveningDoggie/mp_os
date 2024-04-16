#include <not_implemented.h>

#include "../include/client_logger_builder.h"
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;


client_logger* _client_logger;

client_logger_builder::client_logger_builder()
{
    _client_logger = new client_logger();

    //throw not_implemented("client_logger_builder::client_logger_builder()", "your code should be here...");
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
    throw not_implemented("client_logger_builder::~client_logger_builder() noexcept", "your code should be here...");
}


logger_builder *client_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    if (stream_file_path.empty()) throw "Empty file path argument";
    std::string absolute_path = get_file_absolute_path(stream_file_path);
    std::map<std::string, std::pair<std::ofstream*, size_t>>* files_streams_all_ptr = &_client_logger->_files_streams_all;
    std::pair<std::ofstream*, size_t>* files_streams_all_pair_ptr = &(_client_logger->_files_streams_all)[absolute_path];
    std::pair<std::ofstream*, std::set<logger::severity>>* files_streams_pair_ptr = &(_client_logger->_files_streams)[absolute_path];

    if (files_streams_all_ptr->find(absolute_path) == files_streams_all_ptr->end())
    {
        files_streams_all_pair_ptr->first = new std::ofstream(absolute_path, std::ios::app); 
        files_streams_all_pair_ptr->second = 0;

        if (!files_streams_all_pair_ptr->first->is_open())
        {
            delete files_streams_all_pair_ptr->first;
            throw "File not found at this path";
        }
    }
    else files_streams_all_pair_ptr->second++;

    files_streams_pair_ptr->first = files_streams_all_pair_ptr->first;
    files_streams_pair_ptr->second.insert(severity);

    return this;
  }



logger_builder *client_logger_builder::add_console_stream(
    logger::severity severity)
{
    /*
    std::list<logger::severity>* severities = &_client_logger->_severity_console;
    severities->push_back(severity);
    */
    return this;
    //throw not_implemented("logger_builder *client_logger_builder::add_console_stream(logger::severity severity)", "your code should be here...");
}

logger_builder* client_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{
    throw not_implemented("logger_builder* client_logger_builder::transform_with_configuration(std::string const &configuration_file_path, std::string const &configuration_path)", "your code should be here...");
}

logger_builder *client_logger_builder::clear()
{
    throw not_implemented("logger_builder *client_logger_builder::clear()", "your code should be here...");
}

logger *client_logger_builder::build() const
{
    return _client_logger;
   // throw not_implemented("logger *client_logger_builder::build() const", "your code should be here...");
}

std::string client_logger_builder::get_file_absolute_path(std::string str)
{
    return str;
}