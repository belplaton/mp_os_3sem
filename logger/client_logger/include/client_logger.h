#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include "client_logger_builder.h"
#include <fstream>
#include <map>

class client_logger final :
    public logger
{

friend class client_logger_builder;

public:

    struct log_stream final
    {
    private:
        std::ofstream* _stream;

    public:
        explicit log_stream(const std::string&);
        log_stream(const log_stream&) = delete;
        log_stream(log_stream&&) noexcept;
        ~log_stream();

        log_stream& operator = (const log_stream&) = delete;
        log_stream& operator = (log_stream&&) noexcept;

        std::ostream& operator << (const std::string&) const;
    };

private:

    static std::map<std::string, std::weak_ptr<log_stream>> _log_streams;

    std::map<std::string, std::pair<std::shared_ptr<log_stream>, logger::severity>> _files_severity;
    logger::severity _console_severity;
    std::string _output_format;

    explicit client_logger(
        std::map<std::string, logger::severity> const&, logger::severity, std::string const&);

    std::string format_string(const std::string& message, logger::severity severity) const;

public:

    client_logger(
        client_logger const& other);

    client_logger& operator=(
        client_logger const& other);

    client_logger(
        client_logger&& other) noexcept;

    client_logger& operator=(
        client_logger&& other) noexcept;

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const* log(
        const std::string& message,
        logger::severity severity) const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H