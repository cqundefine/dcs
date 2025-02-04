#include "UIElement.h"

namespace DCS
{

UIElement::UIElement(Ref<Renderer> renderer, Position position, std::optional<std::uint32_t> width, std::optional<std::uint32_t> height)
    : m_renderer{renderer}
    , m_position{position}
    , m_width{width}
    , m_height{height}
{
}

Position UIElement::get_relative_mouse_position() const
{
    return m_renderer->target_window()->mouse_position() - m_position;
}

std::uint32_t UIElement::width() const
{
    if (m_width)
        return *m_width;

    return m_renderer->target_window()->width() - m_position.x;
}

std::uint32_t UIElement::height() const
{
    if (m_height)
        return *m_height;

    return m_renderer->target_window()->height() - m_position.y;
}

}
