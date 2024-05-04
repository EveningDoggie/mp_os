#include <not_implemented.h>

#include "../include/client_logger.h"
#include <map>

std::map<std::string, std::pair<std::ofstream*, size_t>> client_logger::_files_streams_all;

client_logger::client_logger() 
{
    _log_format_mask  = "[%s][%d %t] %m";
}

client_logger::client_logger(
    std::map<std::string, std::set<logger::severity>> files_streams_local,
    std::set<logger::severity> console_streams_local,
    std::string log_format_mask
)
{
    _log_format_mask = log_format_mask;
    _console_streams_local = console_streams_local;

    for (auto file_stream : files_streams_local)
    {
        auto path = &file_stream.first;
        auto severitys = &file_stream.second;
        std::ofstream* out;

        if (_files_streams_all.find(*path) == _files_streams_all.end())
        {
            out = new std::ofstream(*path, std::ios::app);

            if (out==NULL || out->is_open() == false)
            {
                clear_streams_all();                                  
                throw "File not found at this path";                 
            }                                                         

            _files_streams_all[*path].first = out;
            _files_streams_all[*path].second = 1;
        } 
        else
        {
            out = _files_streams_all[*path].first;
            _files_streams_all[*path].second++;
        }

        _files_streams_local[*path] = std::pair{ out, *severitys };
    }
}


client_logger::client_logger(
    client_logger const &other)
{
    copy_from_other(other);
}

client_logger &client_logger::operator=(
    client_logger const &other)
{
    if (&other != this)
    {
        clear_streams_all();
        copy_from_other(other);
    }

    return *this;
}

client_logger::client_logger(
    client_logger &&other) noexcept
{
    move_from_other(std::move(other));
}

client_logger &client_logger::operator=(
    client_logger &&other) noexcept
{
    if (&other != this)
    {
        clear_streams_all();
        move_from_other(std::move(other));
    }

    return *this;
}

client_logger::~client_logger() noexcept
{
    clear_streams_all();
}

void client_logger::copy_from_other(client_logger const& other)
{
    _log_format_mask = other._log_format_mask;
    _console_streams_local = other._console_streams_local;
    _files_streams_local = other._files_streams_local;
}

void client_logger::move_from_other(client_logger&& other)
{
    _log_format_mask = other._log_format_mask;
    _console_streams_local = other._console_streams_local;
    _files_streams_local = other._files_streams_local;
}

void client_logger::clear_streams_all() 
{
    _mutex.lock();

    for (auto file_stream : _files_streams_local)
    {
        auto stream_data = &_files_streams_all[file_stream.first];
        if (stream_data->second == 1)
        {
            if (stream_data->first != NULL) 
            {
                stream_data->first->close();
                delete stream_data->first;
            }
            _files_streams_all.erase(file_stream.first);
        }
        else stream_data->second--;
    }

    _mutex.unlock();
}

logger const* client_logger::log(
    const std::string& text,
    logger::severity severity) const noexcept
{
    std::string msg = string_format(_log_format_mask, severity, text);

    for (auto strm : _files_streams_local) 
    {
        std::ofstream* out = strm.second.first;
        if (strm.second.second.find(severity) != strm.second.second.end()) 
        {
            *out << msg << std::endl;
        }
    }

    if (_console_streams_local.find(severity) != _console_streams_local.end())
    {
        std::cout << msg << std::endl;
    }

    
    return this;
}

std::string client_logger::string_format(std::string output_message, logger::severity severity, std::string msg) const
{
    output_message.replace(output_message.find("%s"), 2, severity_to_string(severity));
    output_message.replace(output_message.find("%m"), 2, msg);
    output_message.replace(output_message.find("%d"), 2, current_date_to_string());
    output_message.replace(output_message.find("%t"), 2, current_time_to_string());
    return output_message;
}
