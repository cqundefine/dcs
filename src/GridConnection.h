#pragma once

#include <glm/glm.hpp>

#include "Position.h"

namespace DCS
{

class Grid;
class GridObject;
class Wire;

class GridConnection
{
public:
    enum class Direction
    {
        Input,
        Output,
    };

    enum class Facing
    {
        Up,
        Left,
        Down,
        Right,
    };

    GridConnection(Position relative_position, Facing facing, Direction direction, GridObject* parent);

    Position grid_space_position() const;

    Position relative_position() const { return m_relative_position; }
    Direction direction() const { return m_direction; }
    GridObject* parent() const { return m_parent; }

    void set_connected_wire(Wire* wire);

    bool input_state(const Grid& grid) const;
    void output_state(bool state, const Grid& grid) const;

private:
    Position m_relative_position;
    Facing m_facing;
    Direction m_direction;

    GridObject* m_parent;
    Wire* m_wire = nullptr;
};

}
