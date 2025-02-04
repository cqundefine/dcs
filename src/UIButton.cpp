#include "UIButton.h"
#include "Mouse.h"

namespace DCS
{

UIButton::UIButton(
    Ref<Renderer> renderer,
    Position position,
    std::optional<std::uint32_t> width,
    std::optional<std::uint32_t> height,
    std::string_view text,
    std::function<void()> callback)
    : UIElement{renderer, position, width, height}
    , m_text{text}
    , m_callback{callback}
{
}

void UIButton::draw() const
{
    renderer()->draw_frame(0, 0, width(), height(), 5, {1.0f, 1.0f, 1.0f, 1.0f});
    renderer()->draw_text_centered(renderer()->default_font(), m_text, width() / 2, height() / 2, 1, {1.0f, 1.0f, 1.0f, 1.0f});
}

void UIButton::on_mouse_event(MouseEvent e, Position mouse_position)
{
    if (e.action() == MouseAction::Release)
        m_callback();
}

}
