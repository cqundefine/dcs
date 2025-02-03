#pragma once

#include "Position.h"
#include "Renderer.h"
#include "WindowEvent.h"

namespace DCS
{

class UIElement
{
public:
    UIElement(Ref<Renderer> renderer, Position position);

    virtual void draw() const = 0;
    virtual void update() {}

    virtual void on_key_event(KeyEvent e) {}
    virtual void on_mouse_event(MouseEvent e, Position mouse_position) {}

    Position get_relative_mouse_position() const;

    Position position() const { return m_position; }

protected:
    Ref<Renderer> renderer() const { return m_renderer; }

private:
    Ref<Renderer> m_renderer;
    Position m_position;
};

}
