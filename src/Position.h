#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>

namespace DCS
{

struct Position
{
    std::uint32_t x;
    std::uint32_t y;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Position, x, y);
};

constexpr bool operator==(const Position& p1, const Position& p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

constexpr bool operator<(const Position& p1, const Position& p2)
{
    return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
}

constexpr bool operator<=(const Position& p1, const Position& p2)
{
    return (p1 < p2) || (p1 == p2);
}

constexpr bool operator>(const Position& p1, const Position& p2)
{
    return !(p1 <= p2);
}

constexpr bool operator>=(const Position& p1, const Position& p2)
{
    return !(p1 < p2);
}

constexpr Position& operator+=(Position& p1, const Position& p2)
{
    p1.x += p2.x;
    p1.y += p2.y;

    return p1;
}

constexpr Position operator+(const Position& p1, const Position& p2)
{
    Position p = p1;
    p += p2;
    return p;
}

constexpr Position& operator-=(Position& p1, const Position& p2)
{
    p1.x -= p2.x;
    p1.y -= p2.y;

    return p1;
}

constexpr Position operator-(const Position& p1, const Position& p2)
{
    Position p = p1;
    p -= p2;
    return p;
}

constexpr Position& operator*=(Position& p1, const Position& p2)
{
    p1.x *= p2.x;
    p1.y *= p2.y;

    return p1;
}

constexpr Position operator*(const Position& p1, const Position& p2)
{
    Position p = p1;
    p *= p2;
    return p;
}

constexpr Position& operator/=(Position& p1, const Position& p2)
{
    p1.x /= p2.x;
    p1.y /= p2.y;

    return p1;
}

constexpr Position operator/(const Position& p1, const Position& p2)
{
    Position p = p1;
    p /= p2;
    return p;
}

}

template <>
struct std::formatter<DCS::Position>
{
    constexpr auto parse(std::format_parse_context& ctx) { return std::cbegin(ctx); }

    auto format(const DCS::Position& obj, std::format_context& ctx) const { return std::format_to(ctx.out(), "({}, {})", obj.x, obj.y); }
};
