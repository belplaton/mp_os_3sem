#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_LOGGER_H

#include <iostream>
#include <vector>

class logger
{

friend class logger_builder;

public:

    using severity_type = unsigned char;
    enum class severity : severity_type
    {
        none = 0,
        trace = 1 << 0,
        debug = 1 << 1,
        information = 1 << 2,
        warning = 1 << 3,
        error = 1 << 4,
        critical = 1 << 5,
        all = trace | debug | information | warning | error | critical
    };

    static const std::vector<severity> severities_list;

    friend logger::severity operator~ (logger::severity a);
    friend logger::severity operator| (logger::severity a, logger::severity b);
    friend logger::severity operator& (logger::severity a, logger::severity b);
    friend logger::severity operator^ (logger::severity a, logger::severity b);
    friend logger::severity& operator|= (logger::severity& a, logger::severity b);
    friend logger::severity& operator&= (logger::severity& a, logger::severity b);
    friend logger::severity& operator^= (logger::severity& a, logger::severity b);

public:

    enum class log_type
    {
        files,
        console
    };

public:

    virtual ~logger() noexcept = default;

public:

    virtual logger const* log(
        std::string const& message,
        logger::severity severity) const noexcept = 0;

public:

    logger const* trace(
        std::string const& message) const noexcept;

    logger const* debug(
        std::string const& message) const noexcept;

    logger const* information(
        std::string const& message) const noexcept;

    logger const* warning(
        std::string const& message) const noexcept;

    logger const* error(
        std::string const& message) const noexcept;

    logger const* critical(
        std::string const& message) const noexcept;

public:

    static std::string severity_to_string(
    logger::severity severity);

    static logger::severity string_to_severity(
        const std::string& value);

    static std::string log_type_to_string(
        logger::log_type log_type);

    static logger::log_type string_to_log_type(
        const std::string& value);

protected:

    static std::string current_datetime_to_string() noexcept;
    static std::string current_date_to_string() noexcept;
    static std::string current_time_to_string() noexcept;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_LOGGER_H