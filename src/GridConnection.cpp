#include "GridConnection.h"
#include "Ensure.h"
#include "Grid.h"
#include "GridObject.h"
#include "Wire.h"

namespace DCS
{

GridConnection::GridConnection(glm::ivec2 relative_position, Facing facing, Direction direction, GridObject* parent)
    : m_relative_position{relative_position}
    , m_facing{facing}
    , m_direction{direction}
    , m_parent{parent}
{
}

glm::ivec2 GridConnection::grid_space_position() const
{
    return m_parent->position() + m_relative_position;
}

void GridConnection::set_connected_wire(Wire* wire)
{
    if (m_wire)
        ensure(!wire, "tried to replace the wire on the connection on ({}, {})", grid_space_position().x, grid_space_position().y);

    m_wire = wire;
}

bool GridConnection::input_state(const Grid& grid) const
{
    ensure(m_direction == Direction::Input, "can't input value on non input connection");

    // TODO: introduce a floating state
    return m_wire ? m_wire->state() : false;
}

void GridConnection::output_state(bool state, const Grid& grid) const
{
    ensure(m_direction == Direction::Output, "can't output value on non output connection");

    if (m_wire)
        m_wire->set_state(state, grid);
}

}
