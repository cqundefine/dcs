#include "Wire.h"

#include <print>

#include "Ensure.h"
#include "Grid.h"
#include "GridConnection.h"
#include "GridObject.h"
#include "nlohmann/json.hpp"

namespace DCS
{

Wire::Wire(const std::vector<std::pair<glm::ivec2, glm::ivec2>>& wires, const std::vector<glm::ivec2>& connections)
    : m_wires{wires}
    , m_connections{connections}
{
}

void Wire::draw(const Grid& grid, const Renderer& renderer)
{
    constexpr auto active_color = glm::vec4{0.0f, 0.5f, 0.0f, 1.0f};
    constexpr auto inactive_color = glm::vec4{0.5f, 0.0f, 0.0f, 1.0f};

    const auto cell_size = grid.cell_size();

    for (const auto& wire : m_wires)
    {
        renderer.draw_rect_between_points(
            wire.first.x * cell_size + cell_size / 4,
            wire.first.y * cell_size + cell_size / 4,
            (wire.second.x + 1) * cell_size - cell_size / 4,
            (wire.second.y + 1) * cell_size - cell_size / 4,
            state() ? active_color : inactive_color);
    }
}

void Wire::update(const Grid& grid)
{
    std::println("Updating Wire");
    ensure(needs_update(), "update should not be called if not needed");

    for (const auto connection_position : m_connections)
    {
        const auto maybe_connection = grid.find_grid_connection(connection_position);
        ensure(maybe_connection.has_value(), "created connection to a nonexistant object");
        const auto connection = maybe_connection.value();
        if (!connection->parent()->needs_update())
        {
            connection->parent()->set_needs_update(true);
            connection->parent()->update(grid);
        }
    }

    set_needs_update(false);
}

nlohmann::json Wire::serialize() const
{
    nlohmann::json j;

    auto wires = nlohmann::json::array();
    for (const auto& wire : m_wires)
    {
        nlohmann::json w;

        nlohmann::json begin;
        begin["x"] = wire.first.x;
        begin["y"] = wire.first.y;
        w["begin"] = begin;

        nlohmann::json end;
        end["x"] = wire.second.x;
        end["y"] = wire.second.y;
        w["end"] = end;

        wires.push_back(w);
    }
    j["wires"] = wires;

    auto connections = nlohmann::json::array();
    for (const auto connection : m_connections)
    {
        nlohmann::json c;
        c["x"] = connection.x;
        c["y"] = connection.y;
        connections.push_back(c);
    }
    j["connections"] = connections;

    return j;
}

void Wire::register_wire_for_grid(const Grid& grid)
{
    for (const auto connection_position : m_connections)
    {
        const auto maybe_connection = grid.find_grid_connection(connection_position);
        ensure(maybe_connection.has_value(), "created connection to a nonexistant object");
        const auto connection = maybe_connection.value();

        connection->set_connected_wire(this);
        if (connection->direction() == GridConnection::Direction::Input)
            connection->parent()->set_needs_update(true);
    }
}

void Wire::add_wire(std::pair<glm::ivec2, glm::ivec2> wire)
{
    m_wires.push_back(wire);
}

void Wire::add_connection(glm::ivec2 connection_position, const Grid& grid)
{
    m_connections.push_back(connection_position);
    const auto maybe_connection = grid.find_grid_connection(connection_position);
    ensure(maybe_connection.has_value(), "created connection to a nonexistant object");
    const auto connection = maybe_connection.value();

    connection->set_connected_wire(this);
    if (connection->direction() == GridConnection::Direction::Input)
        connection->parent()->set_needs_update(true);
}

bool Wire::contains_point(glm::ivec2 point) const
{
    for (const auto& wire : m_wires)
    {
        if (point.x >= wire.first.x && point.y >= wire.first.y && point.x <= wire.second.x && point.y <= wire.second.y)
            return true;
    }

    return false;
}

void Wire::set_state(bool state, const Grid& grid)
{
    m_state = state;
    if (!needs_update())
    {
        set_needs_update(true);
        update(grid);
    }
}

}
