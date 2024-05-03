#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H

#include <logger_builder.h>
#include <client_logger.h>
#include <filesystem>
#include <set>
#include <map>

class client_logger_builder final :
    public logger_builder
{

private:

    std::map<std::string, std::set<logger::severity>> _files_streams;

    std::set<logger::severity> _console_streams;

    std::string _log_format_mask = "[%s][%d %t] %m";

private:

    std::string get_file_absolute_path(std::string);

public:

    client_logger_builder();                             

    client_logger_builder(
        client_logger_builder const &other);           

    client_logger_builder &operator=(
        client_logger_builder const &other);            

    client_logger_builder(
        client_logger_builder &&other) noexcept;       

    client_logger_builder &operator=(
        client_logger_builder &&other) noexcept;         

    ~client_logger_builder() noexcept override;        

public:
    
    logger_builder *add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity) override;

    logger_builder* add_file_stream_current_severity(
        std::string const& stream_file_path,
        logger::severity severity) override;

    logger_builder* add_file_stream(
        std::string const& stream_file_path,
        std::set<logger::severity> severity) override;

    logger_builder *add_console_stream(
        logger::severity severity) override;

    logger_builder* add_console_stream_current_severity(
        logger::severity severity) override;

    logger_builder* add_console_stream(
        std::set<logger::severity> severity) override;

    std::set<logger::severity> get_set_from_minimal_severity(logger::severity);

    logger_builder* transform_with_configuration(
        std::string const &configuration_file_path) override;


    logger_builder* change_message_mask(
        std::string const& mask) override;

    logger_builder *clear() override;

    [[nodiscard]] logger *build() const override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H