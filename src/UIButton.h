#pragma once

#include <functional>

#include "UIElement.h"

namespace DCS
{

class UIButton : public UIElement
{
public:
    UIButton(
        Ref<Renderer> renderer,
        Position position,
        std::optional<std::uint32_t> width,
        std::optional<std::uint32_t> height,
        std::string_view text,
        std::function<void()> callback);

    virtual void draw() const override;

    virtual void on_mouse_event(MouseEvent e, Position mouse_position) override;

private:
    std::string m_text;
    std::function<void()> m_callback;
};

}
