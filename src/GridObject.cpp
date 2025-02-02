#include "GridObject.h"
#include "Grid.h"

namespace DCS
{

GridObject::GridObject(Position position)
    : m_position{position}
{
}

void GridObject::draw_connections(const Grid& grid, const Renderer& renderer) const
{
    const auto cell_size = grid.cell_size();
    const auto half_cell_size = cell_size / 2;

    const auto x = position().x * cell_size;
    const auto y = position().y * cell_size;

    for (const auto& position : m_connections)
    {
        auto input_x = x + position.relative_position().x * cell_size + half_cell_size / 2;
        auto input_y = y + position.relative_position().y * cell_size + half_cell_size / 2;

        renderer.draw_rect(input_x, input_y, cell_size - half_cell_size, cell_size - half_cell_size, {0.0f, 0.0f, 0.0f, 1.0f});
    }
}

std::optional<GridConnection*> GridObject::find_grid_connection(Position relative_position)
{
    for (auto& connection : m_connections)
    {
        if (connection.relative_position() == relative_position)
            return &connection;
    }

    return {};
}

}
