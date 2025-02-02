#include "Grid.h"
#include "Mouse.h"

#include <print>
#include <type_traits>

namespace DCS
{

void Grid::add_object(Ref<GridObject> object)
{
    m_objects.push_back(object);
    object->set_needs_update(true);
}

void Grid::add_wire(Ref<Wire> wire)
{
    m_wires.push_back(wire);
    wire->register_wire_for_grid(*this);
}

void Grid::draw(Ref<Renderer> renderer) const
{
    const auto grid_size = std::max(renderer->target_window()->width(), renderer->target_window()->height()) / cell_size() + 1;
    renderer->draw_grid(1, 1, grid_size, cell_size(), 0.5f, {1.0f, 1.0f, 1.0f, 1.0f});

    for (const auto wire : m_wires)
        wire->draw(*this, *renderer);

    for (const auto object : m_objects)
        object->draw(*this, *renderer);
}

void Grid::process_event(WindowEvent event, Ref<Window> window)
{
    std::visit(
        [&](auto&& arg)
        {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::same_as<T, DCS::MouseEvent>)
            {
                if (arg.action() == MouseAction::Press && arg.button() == MouseButton::Left)
                {
                    m_drawing_wire = true;
                    m_drawing_begin = window->mouse_position() / glm::ivec2{cell_size(), cell_size()};
                }
                else if (arg.action() == MouseAction::Release && arg.button() == MouseButton::Left)
                {
                    m_drawing_wire = false;
                    create_wire_from_to(m_drawing_begin, window->mouse_position() / glm::ivec2{cell_size(), cell_size()});
                }
            }
        },
        event);
}

void Grid::update()
{
    for (const auto object : m_objects)
    {
        if (object->needs_update())
            object->update(*this);
    }
}

nlohmann::json Grid::serialize() const
{
    nlohmann::json j;

    auto objects = nlohmann::json::array();
    for (const auto object : m_objects)
        objects.push_back(object->serialize());
    j["objects"] = objects;

    auto wires = nlohmann::json::array();
    for (const auto wire : m_wires)
        wires.push_back(wire->serialize());
    j["wires"] = wires;

    return j;
}

std::optional<Ref<GridObject>> Grid::find_grid_object(glm::ivec2 position) const
{
    for (const auto object : m_objects)
    {
        if (position.x >= object->position().x && position.x <= object->position().x + object->width() - 1 &&
            position.y >= object->position().y && position.y <= object->position().y + object->height() - 1)
        {
            return object;
        }
    }

    return {};
}

std::optional<GridConnection*> Grid::find_grid_connection(glm::ivec2 position) const
{
    const auto maybe_object = find_grid_object(position);
    if (!maybe_object.has_value())
        return {};
    const auto object = maybe_object.value();
    return object->find_grid_connection(position - object->position());
}

std::optional<Ref<Wire>> Grid::find_wire(glm::ivec2 position) const
{
    for (const auto wire : m_wires)
    {
        if (wire->contains_point(position))
            return wire;
    }

    return {};
}

void Grid::create_wire_from_to(glm::ivec2 begin, glm::ivec2 end)
{
    if (const auto maybe_wire = find_wire(begin); maybe_wire.has_value())
    {
        const auto wire = maybe_wire.value();
        wire->add_wire({begin, end});
        if (find_grid_connection(end).has_value())
            wire->add_connection(end, *this);
    }
    else if (const auto maybe_wire = find_wire(end); maybe_wire.has_value())
    {
        const auto wire = maybe_wire.value();
        wire->add_wire({end, begin});
        if (find_grid_connection(begin).has_value())
            wire->add_connection(begin, *this);
    }
    else
    {
        std::vector<glm::ivec2> connections;
        if (find_grid_connection(begin).has_value())
            connections.push_back(begin);
        if (find_grid_connection(end).has_value())
            connections.push_back(end);

        auto wire = MakeRef<Wire>(std::vector{std::make_pair(begin, end)}, std::move(connections));
        add_wire(wire);
        wire->set_needs_update(true);
        wire->update(*this);
    }
}

}
