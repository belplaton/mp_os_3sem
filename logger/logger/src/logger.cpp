#include "../include/logger.h"
#include <iomanip>
#include <sstream>
#include <algorithm>

const std::vector<logger::severity> logger::severities_list
{
    logger::severity::none,
    logger::severity::trace,
    logger::severity::debug,
    logger::severity::information,
    logger::severity::warning,
    logger::severity::error,
    logger::severity::critical
};

logger::severity operator~ (logger::severity a) { return (logger::severity)~(logger::severity_type)a; }
logger::severity operator| (logger::severity a, logger::severity b) { return (logger::severity)((logger::severity_type)a | (logger::severity_type)b); }
logger::severity operator& (logger::severity a, logger::severity b) { return (logger::severity)((logger::severity_type)a & (logger::severity_type)b); }
logger::severity operator^ (logger::severity a, logger::severity b) { return (logger::severity)((logger::severity_type)a ^ (logger::severity_type)b); }
logger::severity& operator|= (logger::severity& a, logger::severity b) { return (logger::severity&)((logger::severity_type&)a |= (logger::severity_type)b); }
logger::severity& operator&= (logger::severity& a, logger::severity b) { return (logger::severity&)((logger::severity_type&)a &= (logger::severity_type)b); }
logger::severity& operator^= (logger::severity& a, logger::severity b) { return (logger::severity&)((logger::severity_type&)a ^= (logger::severity_type)b); }

logger const *logger::trace(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::trace);
}

logger const *logger::debug(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::debug);
}

logger const *logger::information(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::information);
}

logger const *logger::warning(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::warning);
}

logger const *logger::error(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::error);
}

logger const *logger::critical(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::critical);
}

std::string logger::severity_to_string(
    logger::severity severity)
{
    switch (severity)
    {
        case logger::severity::trace:
            return "TRACE";
        case logger::severity::debug:
            return "DEBUG";
        case logger::severity::information:
            return "INFORMATION";
        case logger::severity::warning:
            return "WARNING";
        case logger::severity::error:
            return "ERROR";
        case logger::severity::critical:
            return "CRITICAL";
    }

    throw std::out_of_range("Invalid severity value");
}

logger::severity logger::string_to_severity(
    const std::string& value)
{
    auto upper = value;
    std::transform(upper.begin(), upper.end(), upper.begin(),
        [](unsigned char c) { return std::toupper(c); });

    if (upper == "TRACE")
    {
        return logger::severity::trace;
    }
    else if (upper == "DEBUG")
    {
        return logger::severity::debug;
    }
    else if (upper == "INFORMATION")
    {
        return logger::severity::information;
    }
    else if (upper == "WARNING")
    {
        return logger::severity::warning;
    }
    else if (upper == "ERROR")
    {
        return logger::severity::error;
    }
    else if (upper == "CRITICAL")
    {
        return logger::severity::critical;
    }

    throw std::out_of_range("Invalid severity value");
}

std::string logger::log_type_to_string(
    logger::log_type log_type)
{
    switch (log_type)
    {
    case logger::log_type::console:
        return "CONSOLE";
    case logger::log_type::files:
        return "FILES";
    }

    throw std::out_of_range("Invalid log_type value");
}

logger::log_type logger::string_to_log_type(
    const std::string& value)
{
    auto upper = value;
    std::transform(upper.begin(), upper.end(), upper.begin(),
        [](unsigned char c) { return std::toupper(c); });

    if (upper == "CONSOLE")
    {
        return logger::log_type::console;
    }
    else if (upper == "FILES")
    {
        return logger::log_type::files;
    }

    throw std::out_of_range("Invalid log_type value");
}

std::string logger::current_datetime_to_string() noexcept
{
    auto time = std::time(nullptr);

    std::ostringstream result_stream;
    result_stream << std::put_time(std::localtime(&time), "%d.%m.%Y %H:%M:%S");

    return result_stream.str();
}

std::string logger::current_date_to_string() noexcept
{
    auto time = std::time(nullptr);

    std::ostringstream result_stream;
    result_stream << std::put_time(std::localtime(&time), "%d.%m.%Y");

    return result_stream.str();
}

std::string logger::current_time_to_string() noexcept
{
    auto time = std::time(nullptr);

    std::ostringstream result_stream;
    result_stream << std::put_time(std::localtime(&time), "%H:%M:%S");

    return result_stream.str();
}