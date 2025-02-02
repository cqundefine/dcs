#pragma once

#include "GridObject.h"

namespace DCS
{

class PowerSource : public GridObject
{
public:
    PowerSource(glm::ivec2 position);

    virtual void draw(const Grid& grid, const Renderer& renderer) const override;

    virtual std::uint32_t width() const override;
    virtual std::uint32_t height() const override;

    virtual void update(const Grid& grid) override;

    virtual nlohmann::json serialize() const override;
};

}
