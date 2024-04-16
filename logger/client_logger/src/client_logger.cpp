#include <not_implemented.h>

#include "../include/client_logger.h"
#include <map>

std::map<std::string, std::pair<std::ofstream*, size_t>> client_logger::_files_streams_all;

client_logger::client_logger() {
    
}

client_logger::client_logger(
    client_logger const &other)
{
    throw not_implemented("client_logger::client_logger(client_logger const &other)", "your code should be here...");
}

client_logger &client_logger::operator=(
    client_logger const &other)
{
    throw not_implemented("client_logger &client_logger::operator=(client_logger const &other)", "your code should be here...");
}

client_logger::client_logger(
    client_logger &&other) noexcept
{
    throw not_implemented("client_logger::client_logger(client_logger &&other) noexcept", "your code should be here...");
}

client_logger &client_logger::operator=(
    client_logger &&other) noexcept
{
    throw not_implemented("client_logger &client_logger::operator=(client_logger &&other) noexcept", "your code should be here...");
}

client_logger::~client_logger() noexcept
{
   // delete _severity_file_patches; //+чистка внутри
   throw not_implemented("client_logger::~client_logger() noexcept", "your code should be here...");
}

std::string client_logger::string_format(std::string output_message, logger::severity severity, std::string msg) const
{
    output_message.replace(output_message.find("%s"), 2, severity_to_string(severity));
    output_message.replace(output_message.find("%m"), 2, msg);
    output_message.replace(output_message.find("%d"), 2, current_date_to_string());
    output_message.replace(output_message.find("%t"), 2, current_time_to_string());
    return output_message;
}

logger const* client_logger::log(
    const std::string& text,
    logger::severity severity) const noexcept
{
    std::string msg = string_format(_log_format, severity, text);

    for (auto strm : _files_streams) 
    {
        std::ofstream* out = strm.second.first;
        if (strm.second.second.find(severity) != strm.second.second.end()) 
        {
            *out << msg << std::endl;
        }
    }

    if (_console_streams.find(severity) != _console_streams.end())
    {
        std::cout << msg << std::endl;
    }

    return this;
}
