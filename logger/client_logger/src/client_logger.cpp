#include <not_implemented.h>

#include "../include/client_logger.h"
#include <map>

std::map<std::string, std::pair<std::ofstream*, size_t>> client_logger::_files_streams_all;

client_logger::client_logger() {
    _log_format = "[%s][%d %t] %m";
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


logger const* client_logger::log(
    const std::string& text,
    logger::severity severity) const noexcept
{


    //Вынести отдельно запись в файл и запись в консоль
    //Сделать поверх еще два метода - один сейчас который алгоритм (конкретный) а второй со свитчем
    //здесь использовать просто
    std::string output_message = _log_format;
    output_message.replace(output_message.find("%s"), 2, severity_to_string(severity));
    output_message.replace(output_message.find("%m"), 2, text);
    output_message.replace(output_message.find("%d"), 2, current_date_to_string());
    output_message.replace(output_message.find("%t"), 2, current_time_to_string());


    for (auto strm : _files_streams) 
    {
        std::ofstream* out = strm.second.first;
        *out << output_message << std::endl;
    }

    /*/if (_severity_file_streams->empty()) {}
    std::map<logger::severity, std::list<std::ofstream*>>* severity_file_streams = _severity_file_streams;
    

    std::list<std::ofstream*>* file_streams = &(*severity_file_streams)[severity];
    for (std::ofstream *out : *file_streams)
    {
        *out << out << std::endl;
    }
    */

    return this;
}

//Изменить так как было ранее - потоки чтобы использовались а не файлы
