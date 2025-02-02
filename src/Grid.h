#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <vector>

#include "GridConnection.h"
#include "GridObject.h"
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

    std::optional<Ref<GridObject>> find_grid_object(glm::ivec2 position) const;
    std::optional<GridConnection*> find_grid_connection(glm::ivec2 position) const;
    std::optional<Ref<Wire>> find_wire(glm::ivec2 position) const;

    std::uint32_t cell_size() const { return c_cell_size; }

private:
    void create_wire_from_to(glm::ivec2 begin, glm::ivec2 end);

    constexpr static std::uint32_t c_cell_size = 20;

    std::vector<Ref<GridObject>> m_objects;
    std::vector<Ref<Wire>> m_wires;

    bool m_drawing_wire = false;
    glm::ivec2 m_drawing_begin;
};

}
