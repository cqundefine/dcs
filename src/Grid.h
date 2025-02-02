#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <vector>

#include "GridConnection.h"
#include "GridObject.h"
#include "Position.h"
#include "Renderer.h"
#include "WindowEvent.h"
#include "Wire.h"

namespace DCS
{

class Grid
{
public:
    void add_object(Ref<GridObject> object);
    void add_wire(Ref<Wire> object);
    void draw(Ref<Renderer> renderer) const;
    void process_event(WindowEvent event, Ref<Window> window);
    void update();

    nlohmann::json serialize() const;
    void deserialize(nlohmann::json json);

    std::optional<Ref<GridObject>> find_grid_object(Position position) const;
    std::optional<GridConnection*> find_grid_connection(Position position) const;
    std::optional<Ref<Wire>> find_wire(Position position) const;

    std::uint32_t cell_size() const { return c_cell_size; }

private:
    void create_wire_from_to(Position begin, Position end);

    constexpr static std::uint32_t c_cell_size = 20;

    std::vector<Ref<GridObject>> m_objects;
    std::vector<Ref<Wire>> m_wires;

    bool m_drawing_wire = false;
    Position m_drawing_begin;
};

}
