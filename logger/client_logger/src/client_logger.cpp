#include <not_implemented.h>

#include "../include/client_logger.h"

std::map<
    logger::severity,
    std::list<std::ofstream>
> severity_file_streams;


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
    throw not_implemented("client_logger::~client_logger() noexcept", "your code should be here...");
}

logger const *client_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{
    if (severity_file_streams.empty())
    {

    }

    std::list<std::ofstream>* file_streams = &severity_file_streams[logger::severity::critical];
    if (file_streams->empty())
    {

    }

    for (auto out = file_streams->begin(); out != file_streams->end(); out++)
    {
        *out << text << std::endl;
    }



    //throw not_implemented("logger const *client_logger::log(const std::string &text, logger::severity severity) const noexcept", "your code should be here...");
}