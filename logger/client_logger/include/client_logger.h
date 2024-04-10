#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include "client_logger_builder.h"

#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include <string>
#include <iomanip>
#include <fstream>


class client_logger final :
    public logger
{


public:
    std::map<
        logger::severity,
        std::list<std::string>
    > * _severity_file_patches;
     
public:

    client_logger();

    client_logger(
        client_logger const &other);

    client_logger &operator=(
        client_logger const &other);

    client_logger(
        client_logger &&other) noexcept;   //конструктор - переслать на тот что выше

    client_logger &operator=(
        client_logger &&other) noexcept;  //переслпть на верхний

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;


};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H