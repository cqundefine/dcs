#pragma once

#include <string_view>

#include "AutoRelease.h"
#include "Exception.h"

namespace DCS
{

template <class... Args>
void ensure(bool predicate, std::string_view msg, Args&&... args)
{
    if (!predicate) [[unlikely]]
    {
        throw Exception(std::vformat(msg, std::make_format_args(args...)));
    }
}

template <class T, class... Args>
void ensure(const AutoRelease<T>& obj, std::string_view msg, Args&&... args)
{
    ensure(!!obj, msg, std::forward<Args>(args)...);
}

}
