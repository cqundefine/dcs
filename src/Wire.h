#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

#include "GridConnection.h"
#include "Position.h"
#include "Renderer.h"

namespace DCS
{

class Wire
{
public:
    void draw(const Grid& grid, const Renderer& renderer);
    void update(const Grid& grid);

    nlohmann::json serialize() const;
    static Ref<Wire> deserialize(nlohmann::json json, const Grid& grid);

    void add_wire(std::pair<Position, Position> wire);
    void add_connection(GridConnection* connection);

    void merge_with(Ref<Wire> other);

    bool contains_point(Position point) const;

    bool state() const { return m_state; }
    void set_state(bool state, const Grid& grid);

    bool needs_update() const { return m_needs_update; }
    void set_needs_update(bool needs_update) { m_needs_update = needs_update; }

private:
    std::vector<std::pair<Position, Position>> m_wires;
    std::vector<GridConnection*> m_connections;

    bool m_state = false;
    bool m_needs_update = false;
};

}
