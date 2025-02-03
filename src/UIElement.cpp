#include "UIElement.h"

namespace DCS
{

UIElement::UIElement(Ref<Renderer> renderer, Position position)
    : m_renderer{renderer}
    , m_position{position}
{
}

Position UIElement::get_relative_mouse_position() const
{
    return m_renderer->target_window()->mouse_position() - m_position;
}

}
