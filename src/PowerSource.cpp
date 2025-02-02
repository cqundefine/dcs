#include "PowerSource.h"

#include <print>

#include "Ensure.h"
#include "Grid.h"
#include "GridConnection.h"

namespace DCS
{

PowerSource::PowerSource(glm::ivec2 position)
    : GridObject(position)
{
    m_connections.push_back(GridConnection{{0, 0}, GridConnection::Facing::Down, GridConnection::Direction::Output, this});
}

void PowerSource::draw(const Grid& grid, const Renderer& renderer) const
{
    const auto cell_size = grid.cell_size();

    const auto x = position().x * cell_size;
    const auto y = position().y * cell_size;

    const auto width = this->width() * cell_size;
    const auto height = this->height() * cell_size;

    renderer.draw_rect(x, y, width, height, {0.0f, 1.0f, 0.0f, 1.0f});

    draw_connections(grid, renderer);
}

std::uint32_t PowerSource::width() const
{
    return 1;
}

std::uint32_t PowerSource::height() const
{
    return 1;
}

void PowerSource::update(const Grid& grid)
{
    std::println("Updating PowerSource at ({}, {})", position().x, position().y);
    ensure(needs_update(), "update should not be called if not needed");
    m_connections[0].output_state(true, grid);
    set_needs_update(false);
}

nlohmann::json PowerSource::serialize() const
{
    nlohmann::json j;
    j["type"] = "PowerSource";

    nlohmann::json pos;
    pos["x"] = position().x;
    pos["y"] = position().y;

    j["position"] = pos;

    return j;
}

}
