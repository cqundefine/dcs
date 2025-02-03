#include "PowerSource.h"

#include <print>

#include "Ensure.h"
#include "Grid.h"
#include "GridConnection.h"

namespace DCS
{

PowerSource::PowerSource(Position position)
    : GridObject(position)
{
    m_connections.push_back(GridConnection{{0, 0}, GridConnection::Facing::Down, GridConnection::Direction::Output, this});
}

void PowerSource::draw(const Grid& grid, const Renderer& renderer) const
{
    const auto cell_size = grid.cell_size();

    const auto width = this->width() * cell_size;
    const auto height = this->height() * cell_size;

    renderer.draw_rect(0, 0, width, height, {0.0f, 1.0f, 0.0f, 1.0f});

    draw_connections(grid, renderer);
}

std::string PowerSource::name() const
{
    return "PowerSource";
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
    std::println("Updating PowerSource at {}", position());
    ensure(needs_update(), "update should not be called if not needed");
    m_connections[0].output_state(true, grid);
    set_needs_update(false);
}

nlohmann::json PowerSource::serialize() const
{
    return {};
}

Ref<GridObject> PowerSource::deserialize(Position position, nlohmann::json object_specific)
{
    return MakeRef<PowerSource>(position);
}

}
