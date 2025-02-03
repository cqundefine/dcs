#include "Wire.h"

#include <print>

#include "Ensure.h"
#include "Exception.h"
#include "Grid.h"
#include "GridConnection.h"
#include "GridObject.h"
#include "Position.h"
#include "nlohmann/json.hpp"

namespace DCS
{

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

    for (const auto connection : m_connections)
    {
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
        wires.push_back(nlohmann::json{{"begin", wire.first}, {"end", wire.second}});
    j["wires"] = wires;

    auto connections = nlohmann::json::array();
    for (const auto connection : m_connections)
        connections.push_back(connection->grid_space_position());
    j["connections"] = connections;

    return j;
}

Ref<Wire> Wire::deserialize(nlohmann::json json, const Grid& grid)
{
    const auto object = MakeRef<Wire>();

    for (const auto& wire : json["wires"])
        object->add_wire({wire["begin"].get<Position>(), wire["end"].get<Position>()});

    for (const auto& connection : json["connections"])
    {
        auto maybe_connection = grid.find_grid_connection(connection.get<Position>());
        ensure(maybe_connection.has_value(), "unknown connection during deserialization");
        object->add_connection(*maybe_connection);
    }

    return object;
}

void Wire::add_wire(std::pair<Position, Position> wire)
{
    if (wire.first.x != wire.second.x && wire.first.y != wire.second.y)
        throw Exception("tried to add an invalid wire from {} to {}", wire.first, wire.second);

    if (wire.first.x < wire.second.x)
        m_wires.push_back(wire);
    else
        m_wires.push_back({wire.second, wire.first});
}

void Wire::add_connection(GridConnection* connection)
{
    m_connections.push_back(connection);
    connection->set_connected_wire(this);
    connection->parent()->set_needs_update(true);
}

void Wire::merge_with(Ref<Wire> other)
{
    for (const auto& wire : other->m_wires)
        add_wire(wire);
    other->m_wires.clear();

    for (const auto connection : other->m_connections)
        add_connection(connection);
    other->m_connections.clear();
}

bool Wire::contains_point(Position point) const
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
