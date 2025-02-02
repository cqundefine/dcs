#pragma once

#include <variant>

#include "Mouse.h"

namespace DCS
{

class MouseEvent
{
public:
    MouseEvent(MouseButton button, MouseAction action)
        : m_button{button}
        , m_action{action}
    {
    }

    MouseButton button() const { return m_button; }
    MouseAction action() const { return m_action; }

private:
    MouseButton m_button;
    MouseAction m_action;
};

using WindowEvent = std::variant<MouseEvent>;

}
