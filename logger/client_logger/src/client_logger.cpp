#include <not_implemented.h>

#include "../include/client_logger.h"




client_logger::client_logger() {
    _severity_file_patches = 
        new std::map <logger::severity, std::list<std::string>>();

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
    throw not_implemented("client_logger::~client_logger() noexcept", "your code should be here...");
}

logger const *client_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{


    std::map<logger::severity, std::list<std::string>>* severity_file_patches = _severity_file_patches;
    if (_severity_file_patches->empty()) {}

    std::list<std::string>* file_patches = &(*severity_file_patches)[severity];
    for (std::string patch : *file_patches)
    {
        std::ofstream out;
        out.open(patch, std::ios::app);
        // text = get_format_output_string(text, severity);
        //ÑÄÅËÀÒÜ ÔÓÍÊÖÈŞ Ñ ÔËÀÃÀÌÈ Ñ ÏĞÎØËÎÃÎ ÑÅÌÅÑÒĞÀ. À ËÓ×Øå èñïîëüçîâàòü ãîòîâóş
        out << text << std::endl;
        out.close();
    }

   
    return this;
    //throw not_implemented("logger const *client_logger::log(const std::string &text, logger::severity severity) const noexcept", "your code should be here...");
}


