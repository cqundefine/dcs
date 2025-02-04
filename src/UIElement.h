#pragma once

#include "Position.h"
#include "Renderer.h"
#include "WindowEvent.h"
#include <cstdint>
#include <optional>

namespace DCS
{

class UIElement
{
public:
    UIElement(Ref<Renderer> renderer, Position position, std::optional<std::uint32_t> width, std::optional<std::uint32_t> height);

    virtual void draw() const = 0;
    virtual void update() {}

    virtual void on_key_event(KeyEvent e) {}
    virtual void on_mouse_event(MouseEvent e, Position mouse_position) {}

    Position get_relative_mouse_position() const;

    Position position() const { return m_position; }

    std::uint32_t width() const;
    std::uint32_t height() const;

protected:
    Ref<Renderer> renderer() const { return m_renderer; }

private:
    Ref<Renderer> m_renderer;
    Position m_position;

    std::optional<std::uint32_t> m_width;
    std::optional<std::uint32_t> m_height;
};

}
