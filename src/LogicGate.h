#pragma once

#include "GridObject.h"

namespace DCS
{

class LogicGate : public GridObject
{
public:
    enum class Type
    {
        AND,
        OR,
        XOR,
        NAND,
        NOR,
        XNOR,
    };

    LogicGate(Position position, Type gate);

    virtual void draw(const Grid& grid, const Renderer& renderer) const override;

    virtual std::string name() const override;
    virtual std::uint32_t width() const override;
    virtual std::uint32_t height() const override;

    virtual void update(const Grid& grid) override;

    virtual nlohmann::json serialize() const override;
    static Ref<GridObject> deserialize(Position position, nlohmann::json object_specific);

private:
    Type m_type;
};

}
