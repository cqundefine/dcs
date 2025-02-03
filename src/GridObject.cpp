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

    for (const auto& position : m_connections)
    {
        auto input_x = position.relative_position().x * cell_size + cell_size / 4;
        auto input_y = position.relative_position().y * cell_size + cell_size / 4;

        renderer.draw_rect(input_x, input_y, cell_size / 2, cell_size / 2, {0.0f, 0.0f, 0.0f, 1.0f});
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
