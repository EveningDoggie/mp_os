#include <not_implemented.h>

#include "../include/client_logger_builder.h"


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

    std::map<
        logger::severity,
        std::list<std::ofstream*>
    > * severity_file_streams = _client_logger->_severity_file_streams;

    std::list<std::ofstream*>* file_streams = &((*severity_file_streams)[severity]);

    std::ofstream * out = new std::ofstream(stream_file_path, std::ios::app);
    file_streams->push_back(out);
  
    asd1 = out;
    _client_logger->asd = asd1;

   // _client_logger->asd = out;
    return this;
   // throw not_implemented("logger_builder *client_logger_builder::add_file_stream(std::string const &stream_file_path, logger::severity severity)", "your code should be here...");
}

logger_builder *client_logger_builder::add_console_stream(
    logger::severity severity)
{
    std::list<logger::severity>* severities = &_client_logger->_severity_console;
    severities->push_back(severity);

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