#include <not_implemented.h>

#include "../include/client_logger.h"
#include <map>

std::ofstream* client_logger::asd;

client_logger::client_logger() {
    
    _severity_file_streams =
        new std::map <logger::severity, std::list<std::ofstream*>>();

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

/*
logger const* client_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{

    //Вынести отдельно запись в файл и запись в консоль
    //Сделать поверх еще два метода - один сейчас который алгоритм (конкретный) а второй со свитчем
    //здесь использовать просто

    std::string output_value = "[%s][%d %t] %m";
    output_value.replace(output_value.find("%s"), 2, severity_to_string(severity));
    output_value.replace(output_value.find("%m"), 2, text);
    output_value.replace(output_value.find("%d"), 2, current_date_to_string());
    output_value.replace(output_value.find("%t"), 2, current_time_to_string());

    if (_severity_file_patches->empty()) {}
    std::map<logger::severity, std::list<std::string>>* severity_file_patches = _severity_file_patches;


    std::list<std::string>* file_patches = &(*severity_file_patches)[severity];
    for (std::string patch : *file_patches)
    {
        std::ofstream out;
        out.open(patch, std::ios::app);
        out << output_value << std::endl;
        out.close();
    }
   
    for (logger::severity iteration_severity : _severity_console) 
    {
        if (iteration_severity == severity)
            std::cout << output_value << std::endl;
    }
    
    return this;
    //throw not_implemented("logger const *client_logger::log(const std::string &text, logger::severity severity) const noexcept", "your code should be here...");
}
*/

logger const* client_logger::log(
    const std::string& text,
    logger::severity severity) const noexcept
{
    //перебор со сравнением северити. если конкретные то == . если 
    //минимальный то <=

    //Вынести отдельно запись в файл и запись в консоль
    //Сделать поверх еще два метода - один сейчас который алгоритм (конкретный) а второй со свитчем
    //здесь использовать просто

    std::string output_value = "[%s][%d %t] %m";
    output_value.replace(output_value.find("%s"), 2, severity_to_string(severity));
    output_value.replace(output_value.find("%m"), 2, text);
    output_value.replace(output_value.find("%d"), 2, current_date_to_string());
    output_value.replace(output_value.find("%t"), 2, current_time_to_string());

    if (_severity_file_streams->empty()) {}
    std::map<logger::severity, std::list<std::ofstream*>>* severity_file_streams = _severity_file_streams;
    

    std::list<std::ofstream*>* file_streams = &(*severity_file_streams)[severity];
    for (std::ofstream *out : *file_streams)
    {
        *out << out << std::endl;
    }


    return this;
    //throw not_implemented("logger const *client_logger::log(const std::string &text, logger::severity severity) const noexcept", "your code should be here...");
}

//Изменить так как было ранее - потоки чтобы использовались а не файлы
