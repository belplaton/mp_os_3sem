#include <not_implemented.h>

#include "../include/client_logger.h"
#include <sstream>

std::map<std::string, std::weak_ptr<client_logger::log_stream>> client_logger::_log_streams = std::map<std::string, std::weak_ptr<client_logger::log_stream>>();

#pragma region client_logger::log_stream

client_logger::log_stream::log_stream(const std::string& path) :
    _stream(nullptr)
{
    try
    {
        _stream = new std::ofstream(path, std::ios::app);
        if (!_stream->is_open())
        {
            throw std::logic_error("File open error");
        }
    }
    catch (const std::bad_alloc& catched)
    {
        _stream == nullptr;
        std::cerr << catched.what() << std::endl;
        throw catched;
    }
    catch (const std::exception& catched)
    {
        delete _stream;
        _stream == nullptr;
        std::cerr << catched.what() << std::endl;
        throw catched;
    }
}

client_logger::log_stream::log_stream(client_logger::log_stream&& other) noexcept :
    _stream(other._stream)
{
    other._stream = nullptr;
}

client_logger::log_stream& client_logger::log_stream::operator = (client_logger::log_stream&& other) noexcept
{
    if (this != &other)
    {
        _stream->flush();
        _stream->close();
        delete _stream;

        _stream = other._stream;
        other._stream = nullptr;
    }

    return *this;
}

std::ostream& client_logger::log_stream::operator<<(std::string const& str) const
{
    return (*_stream << str).flush();
}

client_logger::log_stream::~log_stream()
{
    delete _stream;
    _stream == nullptr;
}

#pragma endregion

client_logger::client_logger(
    const std::map<std::string, logger::severity>& files_severity, logger::severity console_severity, const std::string& output_format):
    _files_severity(),
    _console_severity(console_severity),
    _output_format(output_format)
{
    auto iter = files_severity.begin();
    while (iter != files_severity.end())
    {
        auto lsiter = _log_streams.find(iter->first);
        if (lsiter != _log_streams.end())
        {
            auto pair = std::make_pair(lsiter->second, iter->second);
            _files_severity.emplace(iter->first, pair);
        }
        else
        {
            auto ls_ptr = std::make_shared<client_logger::log_stream>(log_stream(iter->first));
            auto pair = std::make_pair(ls_ptr, iter->second);
            _files_severity.emplace(iter->first, pair);
            _log_streams.emplace(iter->first, ls_ptr);
        }

        iter++;
    }
}

client_logger::client_logger(
    client_logger const& other):
    _files_severity(other._files_severity),
    _console_severity(other._console_severity),
    _output_format(other._output_format)
{
    //throw not_implemented("client_logger::client_logger(client_logger const &other)", "your code should be here...");
}

client_logger& client_logger::operator=(
    client_logger const& other)
{
    if (this != &other)
    {
        _files_severity = other._files_severity;
        _console_severity = other._console_severity;
        _output_format = other._output_format;
    }

    return *this;
    //throw not_implemented("client_logger &client_logger::operator=(client_logger const &other)", "your code should be here...");
}

client_logger::client_logger(
    client_logger&& other) noexcept:
    _files_severity(std::move(other._files_severity)),
    _console_severity(std::move(other._console_severity)),
    _output_format(std::move(other._output_format))
{
    //throw not_implemented("client_logger::client_logger(client_logger &&other) noexcept", "your code should be here...");
}

client_logger& client_logger::operator=(
    client_logger&& other) noexcept
{
    if (this != &other)
    {
        _files_severity = std::move(other._files_severity);
        _console_severity = std::move(other._console_severity);
        _output_format = std::move(other._output_format);
    }

    return *this;
    //throw not_implemented("client_logger &client_logger::operator=(client_logger &&other) noexcept", "your code should be here...");
}

client_logger::~client_logger() noexcept
{
    auto iter = _files_severity.begin();
    while (iter != _files_severity.end())
    {
        auto pair = _log_streams.find(iter->first);
        if (pair != _log_streams.end())
        {
            iter->second.first = nullptr;
            if (pair->second.expired())
            {
                _log_streams.erase(iter->first);
            }
        }
        else
        {
            throw std::logic_error("Cant found log_stream of client_logger in static!");
        }

        iter++;
    }

    _files_severity.clear();
    //throw not_implemented("client_logger::~client_logger() noexcept", "your code should be here...");
}

logger const* client_logger::log(
    const std::string& text,
    logger::severity severity) const noexcept
{
    for (auto i = 0; i < logger::severities_list.size(); i++)
    {
        auto temp_severity = logger::severities_list[i];
        if ((temp_severity & severity) != logger::severity::none)
        {
            if ((_console_severity & temp_severity) != logger::severity::none)
            {
                std::cerr << format_string(text, temp_severity) << std::endl;
            }

            auto iter = _files_severity.begin();
            while (iter != _files_severity.end())
            {
                if ((iter->second.second & temp_severity) != logger::severity::none)
                {
                    *(iter->second.first) << format_string(text, temp_severity) << std::endl;
                }

                iter++;
            }
        }
    }

    return this;
    //throw not_implemented("logger const *client_logger::log(const std::string &text, logger::severity severity) const noexcept", "your code should be here...");
}

std::string client_logger::format_string(const std::string& message, logger::severity severity) const
{
    std::stringstream sresult;
    auto iter = _output_format.begin();
    while (iter != _output_format.end())
    {
        switch (*iter)
        {
        case '%':
            iter++;
            if (iter == _output_format.end())
            {
                throw std::logic_error("Format error! Format pointers should starts with '%' and ends with modificator type");
            }

            switch (*iter)
            {
            case 'd':
                sresult << current_date_to_string();
                break;
            case 't':
                sresult << current_time_to_string();
                break;
            case 's':
                sresult << logger::severity_to_string(severity);
                break;
            case 'm':
                sresult << message;
                break;
            default:
                throw std::logic_error("Unknown format modificator");
            }

            break;
        default:
            sresult << *iter;
            break;
        }

        iter++;
    }

    auto result = sresult.str();
    return result;
}