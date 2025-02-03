#include "LogicGate.h"

#include <print>

#include "Ensure.h"
#include "Exception.h"
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

static constexpr DCS::LogicGate::Type name_to_logic_gate_type(std::string type)
{
    using enum DCS::LogicGate::Type;
    if (type == "AND")
        return AND;
    else if (type == "OR")
        return OR;
    else if (type == "XOR")
        return XOR;
    else if (type == "NAND")
        return NAND;
    else if (type == "NAND")
        return NAND;
    else if (type == "XNOR")
        return XNOR;
    else
        throw DCS::Exception("invalid logic gate type: {}", type);
}

namespace DCS
{

LogicGate::LogicGate(Position position, LogicGate::Type type)
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

    const auto width = this->width() * cell_size;
    const auto height = this->height() * cell_size;

    renderer.draw_rect(0, 0, width, height, {1.0f, 1.0f, 1.0f, 1.0f});
    renderer.draw_text_centered(renderer.default_font(), logic_gate_type_name(m_type), width / 2, height / 2, 1, {0.0f, 0.0f, 0.0f, 1.0f});

    draw_connections(grid, renderer);
}

std::string LogicGate::name() const
{
    return "LogicGate";
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
    std::println("Updating LogicGate at {}", position());
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
    j["type"] = logic_gate_type_name(m_type);
    return j;
}

Ref<GridObject> LogicGate::deserialize(Position position, nlohmann::json object_specific)
{
    auto type = name_to_logic_gate_type(object_specific["type"]);
    return MakeRef<LogicGate>(position, type);
}

}
