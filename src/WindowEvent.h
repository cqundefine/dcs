#pragma once

#include <variant>

#include "Keyboard.h"
#include "Mouse.h"

namespace DCS
{

class KeyEvent
{
public:
    KeyEvent(int key, KeyboardAction action, KeyboardModifiers modifiers);

    int key() const { return m_key; }
    KeyboardAction action() const { return m_action; }
    KeyboardModifiers modifiers() const { return m_modifiers; }

private:
    int m_key;
    KeyboardAction m_action;
    KeyboardModifiers m_modifiers;
};

class MouseEvent
{
public:
    MouseEvent(MouseButton button, MouseAction action);

    MouseButton button() const { return m_button; }
    MouseAction action() const { return m_action; }

private:
    MouseButton m_button;
    MouseAction m_action;
};

using WindowEvent = std::variant<KeyEvent, MouseEvent>;

}
