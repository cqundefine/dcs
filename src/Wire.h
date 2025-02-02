#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

#include "GridConnection.h"
#include "Renderer.h"

namespace DCS
{

class Wire
{
public:
    Wire(const std::vector<std::pair<glm::ivec2, glm::ivec2>>& wires, const std::vector<glm::ivec2>& connections);

    void draw(const Grid& grid, const Renderer& renderer);
    void update(const Grid& grid);

    nlohmann::json serialize() const;

    void register_wire_for_grid(const Grid& grid); // FIXME: Make sure this only gets called by the grid
    void add_wire(std::pair<glm::ivec2, glm::ivec2> wire);
    void add_connection(glm::ivec2 connection_position, const Grid& grid);

    bool contains_point(glm::ivec2 point) const;

    bool state() const { return m_state; }
    void set_state(bool state, const Grid& grid);

    bool needs_update() const { return m_needs_update; }
    void set_needs_update(bool needs_update) { m_needs_update = needs_update; }

private:
    std::vector<std::pair<glm::ivec2, glm::ivec2>> m_wires;
    std::vector<glm::ivec2> m_connections;

    bool m_state = false;
    bool m_needs_update = false;
};

}
