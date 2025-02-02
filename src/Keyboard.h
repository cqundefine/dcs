#pragma once

#include <type_traits>
namespace DCS
{

enum class KeyboardAction
{
    Release = 0,
    Press = 1,
    Repeat = 2,
};

enum class KeyboardModifiers
{
    Shift = 0x0001,
    Control = 0x0002,
    Alt = 0x0004,
    Super = 0x0008,
};

constexpr KeyboardModifiers operator|(KeyboardModifiers lhs, KeyboardModifiers rhs)
{
    using Type = std::underlying_type_t<KeyboardModifiers>;
    return static_cast<KeyboardModifiers>(static_cast<Type>(lhs) | static_cast<Type>(rhs));
}

}
