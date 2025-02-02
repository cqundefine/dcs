#include "Grid.h"
#include "GridConnection.h"
#include "Keyboard.h"
#include "LogicGate.h"
#include "Mouse.h"
#include "PowerSource.h"
#include "Util.h"
#include "WindowEvent.h"

#include <functional>
#include <print>
#include <type_traits>
#include <vector>

static std::map<std::string, std::function<DCS::Ref<DCS::GridObject>(DCS::Position, nlohmann::json)>> g_object_deserializers = {
    {"PowerSource", DCS::PowerSource::deserialize},
    {"LogicGate", DCS::LogicGate::deserialize}};

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

            if constexpr (std::same_as<T, KeyEvent>)
            {
                if (arg.key() == GLFW_KEY_S && arg.action() == KeyboardAction::Press && arg.modifiers() == KeyboardModifiers::Control)
                {
                    std::println("{}", serialize().dump(4));
                }
            }
            else if constexpr (std::same_as<T, MouseEvent>)
            {
                if (arg.action() == MouseAction::Press && arg.button() == MouseButton::Left)
                {
                    m_drawing_wire = true;
                    m_drawing_begin = window->mouse_position() / Position{cell_size(), cell_size()};
                }
                else if (arg.action() == MouseAction::Release && arg.button() == MouseButton::Left)
                {
                    m_drawing_wire = false;
                    create_wire_from_to(m_drawing_begin, window->mouse_position() / Position{cell_size(), cell_size()});
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
    {
        nlohmann::json o;
        o["name"] = object->name();
        o["position"] = object->position();
        o["object_specific"] = object->serialize();

        objects.push_back(o);
    }

    j["objects"] = objects;

    auto wires = nlohmann::json::array();
    for (const auto wire : m_wires)
        wires.push_back(wire->serialize());
    j["wires"] = wires;

    return j;
}

void Grid::deserialize(nlohmann::json json)
{
    for (const auto& object : json["objects"])
    {
        std::string name = object["name"].get<std::string>();
        Position position = object["position"].get<Position>();
        add_object(g_object_deserializers[name](position, object["object_specific"]));
    }

    for (const auto& wire : json["wires"])
        add_wire(Wire::deserialize(wire, *this));
}

std::optional<Ref<GridObject>> Grid::find_grid_object(Position position) const
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

std::optional<GridConnection*> Grid::find_grid_connection(Position position) const
{
    const auto maybe_object = find_grid_object(position);
    if (!maybe_object.has_value())
        return {};
    const auto object = maybe_object.value();
    return object->find_grid_connection(position - object->position());
}

std::optional<Ref<Wire>> Grid::find_wire(Position position) const
{
    for (const auto wire : m_wires)
    {
        if (wire->contains_point(position))
            return wire;
    }

    return {};
}

void Grid::create_wire_from_to(Position begin, Position end)
{
    if (begin.x != end.x && begin.y != end.y)
        return;

    const auto maybe_wire_begin = find_wire(begin);
    const auto maybe_wire_end = find_wire(end);

    if (maybe_wire_begin && maybe_wire_end)
    {
        const auto wire_begin = maybe_wire_begin.value();
        const auto wire_end = maybe_wire_end.value();

        wire_begin->merge_with(wire_end);
        std::erase(m_wires, wire_end);

        wire_begin->add_wire({begin, end});
    }
    else if (maybe_wire_begin)
    {
        const auto wire = maybe_wire_begin.value();
        wire->add_wire({begin, end});
        if (auto maybe_connection = find_grid_connection(end); maybe_connection.has_value())
            wire->add_connection(*maybe_connection);
    }
    else if (maybe_wire_end)
    {
        const auto wire = maybe_wire_end.value();
        wire->add_wire({end, begin});
        if (auto maybe_connection = find_grid_connection(begin); maybe_connection.has_value())
            wire->add_connection(*maybe_connection);
    }
    else
    {
        auto wire = MakeRef<Wire>();
        wire->add_wire(std::make_pair(begin, end));

        if (auto maybe_connection = find_grid_connection(begin); maybe_connection.has_value())
            wire->add_connection(*maybe_connection);
        if (auto maybe_connection = find_grid_connection(end); maybe_connection.has_value())
            wire->add_connection(*maybe_connection);

        add_wire(wire);
        wire->set_needs_update(true);
        wire->update(*this);
    }
}

}
