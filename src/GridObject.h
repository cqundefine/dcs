#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "GridConnection.h"
#include "Renderer.h"

namespace DCS
{

class Grid;

class GridObject
{
public:
    GridObject(glm::ivec2 position);

    virtual void draw(const Grid& grid, const Renderer& renderer) const = 0;

    virtual std::uint32_t width() const = 0;
    virtual std::uint32_t height() const = 0;

    virtual void update(const Grid& grid) = 0;

    virtual nlohmann::json serialize() const = 0;

    std::optional<GridConnection*> find_grid_connection(glm::ivec2 relative_position);

    glm::ivec2 position() const { return m_position; }

    bool needs_update() const { return m_needs_update; }
    void set_needs_update(bool needs_update) { m_needs_update = needs_update; }

protected:
    void draw_connections(const Grid& grid, const Renderer& renderer) const;

    std::vector<GridConnection> m_connections;

private:
    glm::ivec2 m_position;
    bool m_needs_update;
};

}
