#include "LogicGate.h"

#include <print>

#include "Ensure.h"
#include "Grid.h"
#include "GridConnection.h"

static constexpr std::string logic_gate_type_name(DCS::LogicGate::Type type)
{
    switch (type)
    {
        using enum DCS::LogicGate::Type;
        case AND:  return "AND";
        case OR:   return "OR";
        case XOR:  return "XOR";
        case NAND: return "NAND";
        case NOR:  return "NAND";
        case XNOR: return "XNOR";
    }
}

static constexpr bool logic_gate_type_operation(DCS::LogicGate::Type type, bool a, bool b)
{
    switch (type)
    {
        using enum DCS::LogicGate::Type;
        case AND:  return a & b;
        case OR:   return a | b;
        case XOR:  return a ^ b;
        case NAND: return ~(a & b);
        case NOR:  return ~(a | b);
        case XNOR: return ~(a ^ b);
    }
}

namespace DCS
{

LogicGate::LogicGate(glm::ivec2 position, LogicGate::Type type)
    : GridObject{position}
    , m_type{type}
{
    m_connections.push_back(GridConnection{{0, 0}, GridConnection::Facing::Left, GridConnection::Direction::Input, this});
    m_connections.push_back(GridConnection{{0, 2}, GridConnection::Facing::Left, GridConnection::Direction::Input, this});

    m_connections.push_back(GridConnection{{3, 1}, GridConnection::Facing::Left, GridConnection::Direction::Output, this});
}

void LogicGate::draw(const Grid& grid, const Renderer& renderer) const
{
    const auto cell_size = grid.cell_size();

    const auto x = position().x * cell_size;
    const auto y = position().y * cell_size;

    const auto width = this->width() * cell_size;
    const auto height = this->height() * cell_size;

    renderer.draw_rect(x, y, width, height, {1.0f, 1.0f, 1.0f, 1.0f});
    renderer.draw_text_centered(
        renderer.default_font(), logic_gate_type_name(m_type), x + width / 2, y + height / 2, 1, {0.0f, 0.0f, 0.0f, 1.0f});

    draw_connections(grid, renderer);
}

std::uint32_t LogicGate::width() const
{
    return 4;
}

std::uint32_t LogicGate::height() const
{
    return 3;
}

void LogicGate::update(const Grid& grid)
{
    std::println("Updating LogicGate at ({}, {})", position().x, position().y);
    ensure(needs_update(), "update should not be called if not needed");

    const auto& input1 = m_connections[0];
    const auto& input2 = m_connections[1];

    const auto& output = m_connections[2];

    output.output_state(logic_gate_type_operation(m_type, input1.input_state(grid), input2.input_state(grid)), grid);

    set_needs_update(false);
}

nlohmann::json LogicGate::serialize() const
{
    nlohmann::json j;
    j["type"] = "LogicGate";

    nlohmann::json pos;
    pos["x"] = position().x;
    pos["y"] = position().y;

    j["position"] = pos;
    j["gate_type"] = logic_gate_type_name(m_type);

    return j;
}

}
