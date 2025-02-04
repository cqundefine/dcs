#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <vector>

#include "GridConnection.h"
#include "GridObject.h"
#include "Position.h"
#include "Renderer.h"
#include "UIElement.h"
#include "WindowEvent.h"
#include "Wire.h"

namespace DCS
{

class Grid : public UIElement
{
public:
    Grid(Ref<Renderer> renderer, Position position, std::optional<std::uint32_t> width, std::optional<std::uint32_t> height);

    void add_object(Ref<GridObject> object);
    void add_wire(Ref<Wire> object);

    virtual void draw() const override;
    virtual void update() override;

    virtual void on_key_event(KeyEvent e) override;
    virtual void on_mouse_event(MouseEvent e, Position mouse_position) override;

    nlohmann::json serialize() const;
    void deserialize(nlohmann::json json);

    std::optional<Ref<GridObject>> find_grid_object(Position position) const;
    std::optional<GridConnection*> find_grid_connection(Position position) const;
    std::optional<Ref<Wire>> find_wire(Position position) const;

    constexpr std::uint32_t cell_size() const { return c_cell_size; }

private:
    void create_wire_from_to(Position begin, Position end);

    constexpr static std::uint32_t c_cell_size = 20;

    std::vector<Ref<GridObject>> m_objects;
    std::vector<Ref<Wire>> m_wires;

    bool m_drawing_wire = false;
    Position m_drawing_begin;
};

}
