#pragma once

#include <functional>

#include "NonCopyable.h"

namespace DCS
{

template <class T, T Invalid = T{}>
class AutoRelease
{
    MAKE_NONCOPYABLE(AutoRelease);

public:
    constexpr AutoRelease()
        : AutoRelease{Invalid, nullptr}
    {
    }

    constexpr AutoRelease(T obj, std::function<void(T)> deleter)
        : m_obj{obj}
        , m_deleter{deleter}
    {
    }

    constexpr ~AutoRelease()
    {
        if (m_obj != Invalid && m_deleter)
            m_deleter(m_obj);
    }

    constexpr AutoRelease(AutoRelease&& other)
        : AutoRelease()
    {
        swap(other);
    }

    constexpr AutoRelease& operator=(AutoRelease&& other)
    {
        AutoRelease new_obj{std::move(other)};
        swap(new_obj);

        return *this;
    }

    constexpr void swap(AutoRelease& other) noexcept
    {
        std::ranges::swap(m_obj, other.m_obj);
        std::ranges::swap(m_deleter, other.m_deleter);
    }

    constexpr T get() const { return m_obj; }

    constexpr operator T() const { return m_obj; }

    constexpr explicit operator bool() const { return m_obj != Invalid; }

    constexpr T* operator&() noexcept { return std::addressof(m_obj); }

private:
    T m_obj;
    std::function<void(T)> m_deleter;
};

}
