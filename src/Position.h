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
