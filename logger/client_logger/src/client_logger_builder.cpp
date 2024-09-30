#include <not_implemented.h>

#include "../include/client_logger_builder.h"
#include <client_logger.h>

client_logger_builder::client_logger_builder() :
    _files_severity(),
    _console_severity(),
    _output_format()
{
    //throw not_implemented("client_logger_builder::client_logger_builder()", "your code should be here...");
}

client_logger_builder::client_logger_builder(
    client_logger_builder const& other) :
    _files_severity(other._files_severity),
    _console_severity(other._console_severity),
    _output_format(other._output_format)
{
    //throw not_implemented("client_logger_builder::client_logger_builder(client_logger_builder const &other)", "your code should be here...");
}

client_logger_builder& client_logger_builder::operator=(
    client_logger_builder const& other)
{
    if (this != &other)
    {
        _files_severity.clear();
        _files_severity = other._files_severity;
        _console_severity = other._console_severity;
        _output_format = other._output_format;
    }

    return *this;
    //throw not_implemented("client_logger_builder &client_logger_builder::operator=(client_logger_builder const &other)", "your code should be here...");
}

client_logger_builder::client_logger_builder(
    client_logger_builder&& other) noexcept :
    _files_severity(std::move(other._files_severity)),
    _console_severity(std::move(other._console_severity)),
    _output_format(std::move(other._output_format))
{
    //throw not_implemented("client_logger_builder::client_logger_builder(client_logger_builder &&other) noexcept", "your code should be here...");
}

client_logger_builder& client_logger_builder::operator=(
    client_logger_builder&& other) noexcept
{
    if (this != &other)
    {
        _files_severity.clear();
        _files_severity = std::move(other._files_severity);
        _console_severity = std::move(other._console_severity);
        _output_format = std::move(other._output_format);
    }

    return *this;
    //throw not_implemented("client_logger_builder &client_logger_builder::operator=(client_logger_builder &&other) noexcept", "your code should be here...");
}

client_logger_builder::~client_logger_builder() noexcept
{
    _files_severity.clear();
    //throw not_implemented("client_logger_builder::~client_logger_builder() noexcept", "your code should be here...");
}

logger_builder* client_logger_builder::add_file_stream(
    std::string const& stream_file_path,
    logger::severity severity)
{
    if (stream_file_path.empty())
    {
        throw std::logic_error("Path to file cath be empty!");
    }

    auto absolute_path = std::filesystem::absolute(stream_file_path).string();
    auto iter = _files_severity.find(absolute_path);
    if (iter != _files_severity.end())
    {
        iter->second |= severity;
    }
    else
    {
        _files_severity.emplace(absolute_path, severity);
    }

    return this;
    //throw not_implemented("logger_builder *client_logger_builder::add_file_stream(std::string const &stream_file_path, logger::severity severity)", "your code should be here...");
}

logger_builder* client_logger_builder::add_console_stream(
    logger::severity severity)
{
    _console_severity |= severity;

    return this;
   // throw not_implemented("logger_builder *client_logger_builder::add_console_stream(logger::severity severity)", "your code should be here...");
}

logger_builder* client_logger_builder::set_output_format(
    std::string const& output_format
)
{
    _output_format = output_format;

    return this;
}

logger_builder* client_logger_builder::transform_with_configuration(
    std::string const& configuration_file_path,
    std::string const& configuration_path)
{
    clear();
    std::ifstream file(configuration_file_path);
    if (!file.is_open())
    {
        throw std::runtime_error("Configuration file not found: " + configuration_file_path);
    }

    json cfg;
    try
    {
        file >> cfg;
    }
    catch (const std::exception e)
    {
        throw std::runtime_error("Error reading configuration file: " + std::string(e.what()));
    }

    json data;
    try
    {
        data = cfg.at(configuration_path);
    }
    catch (const std::exception e)
    {
        throw std::runtime_error("Configuration path not found: " + configuration_path);
    }

    if (cfg.contains("format"))
    {
        _output_format = cfg["format"];
    }

    if (cfg.contains("log_types"))
    {
        for (const auto& log_type_entry : cfg["log_types"])
        {
            for (const auto& [log_type_name, log_type_cfg] : log_type_entry.items())
            {
                try
                {
                    auto log_type = logger::string_to_log_type(log_type_name);
                    switch (log_type)
                    {
                    case (logger::log_type::console):
                        if (log_type_cfg.contains("severity"))
                        {
                            auto severity = logger::severity::none;
                            for (const auto& severity_name : log_type_cfg["severity"].items())
                            {
                                severity |= logger::string_to_severity(severity_name.value());
                            }

                            add_console_stream(severity);
                        }
                        break;
                    case (logger::log_type::files):
                        for (const auto& [file_name, file_cfg] : log_type_cfg.items())
                        {
                            if (log_type_cfg.contains("severity"))
                            {
                                auto severity = logger::severity::none;
                                for (const auto& severity_name : log_type_cfg["severity"].items())
                                {
                                    severity |= logger::string_to_severity(severity_name.value());
                                }

                                add_file_stream(file_name, severity);
                            }
                        }
                        break;
                    }
                }
                catch (...)
                {
                    continue;
                }
            }
        }
    }

    return this;

    //throw not_implemented("logger_builder* client_logger_builder::transform_with_configuration(std::string const &configuration_file_path, std::string const &configuration_path)", "your code should be here...");
}

logger_builder* client_logger_builder::clear()
{
    _files_severity.clear();
    _output_format.clear();
    _console_severity = logger::severity::none;
    return this;
    //throw not_implemented("logger_builder *client_logger_builder::clear()", "your code should be here...");
}

logger* client_logger_builder::build() const
{
    return new client_logger(_files_severity, _console_severity, _output_format);
    //throw not_implemented("logger *client_logger_builder::build() const", "your code should be here...");
}