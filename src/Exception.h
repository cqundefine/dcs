#pragma once

#include <format>
#include <stacktrace>
#include <stdexcept>
#include <string>

namespace DCS
{

class Exception : public std::runtime_error
{
public:
    template <class... Args>
    Exception(std::string_view msg, Args&&... args)
        : std::runtime_error{std::vformat(msg, std::make_format_args(args...))}
        , m_trace{std::stacktrace::current()}
    {
    }

    std::string stacktrace() const;

private:
    std::stacktrace m_trace;
};

}

template <>
struct std::formatter<DCS::Exception>
{
    constexpr auto parse(std::format_parse_context& ctx) { return std::cbegin(ctx); }

    auto format(const DCS::Exception& obj, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "{}\n{}", obj.what(), obj.stacktrace());
    }
};
