#include "WindowEvent.h"

namespace DCS
{

KeyEvent::KeyEvent(int key, KeyboardAction action, KeyboardModifiers modifiers)
    : m_key{key}
    , m_action{action}
    , m_modifiers{modifiers}
{
}

MouseEvent::MouseEvent(MouseButton button, MouseAction action)
    : m_button{button}
    , m_action{action}
{
}

}
