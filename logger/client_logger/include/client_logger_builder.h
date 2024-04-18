#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H

#include <logger_builder.h>
#include <client_logger.h>
#include <filesystem>
#include <set>

class client_logger_builder final :
    public logger_builder
{

private:

    std::set<std::string> _saved_patches;
    client_logger * _client_logger;

private:

    std::string get_file_absolute_path(std::string);

public:

    client_logger_builder();                             //конструктор

    client_logger_builder(
        client_logger_builder const &other);            //конструктор

    client_logger_builder &operator=(
        client_logger_builder const &other);            //

    client_logger_builder(
        client_logger_builder &&other) noexcept;        //конструктор - переслать на тот что выше

    client_logger_builder &operator=(
        client_logger_builder &&other) noexcept;          //переслпть на верхний

    ~client_logger_builder() noexcept override;         //деструктор

public:
    
    logger_builder *add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity) override;

    logger_builder* add_file_stream(
        std::string const& stream_file_path,
        std::set<logger::severity> severity);

    logger_builder *add_console_stream(
        logger::severity severity) override;

    logger_builder* add_console_stream(
        std::set<logger::severity> severity);

    std::set<logger::severity> get_set_from_minimal_severity(logger::severity);

    logger_builder* transform_with_configuration(
        std::string const &configuration_file_path) override;

    logger_builder *clear() override;

    [[nodiscard]] logger *build() const override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H