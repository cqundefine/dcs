#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <print>
#include <string_view>

#include "Ensure.h"
#include "Grid.h"
#include "GridConnection.h"
#include "Position.h"
#include "Renderer.h"
#include "UIElement.h"
#include "Util.h"
#include "Window.h"
#include "WindowEvent.h"

int main(int argc, char** argv)
{
    try
    {
        DCS::ensure(argc == 2, "usage: {} <assets>", argv[0]);
        const auto assets_path = std::filesystem::path{argv[1]};

        const auto window = DCS::MakeRef<DCS::Window>(800, 600);
        const auto renderer = DCS::MakeRef<DCS::Renderer>(window, assets_path);

        std::vector<DCS::Ref<DCS::UIElement>> ui_elements;

        const auto grid = DCS::MakeRef<DCS::Grid>(renderer, DCS::Position{150, 0});
        ui_elements.push_back(grid);

        std::ifstream f(assets_path / "test_scene.json");
        grid->deserialize(nlohmann::json::parse(f));

        while (!window->should_close())
        {
            auto event = window->poll_event();
            while (event)
            {
                std::visit(
                    [&](auto&& e)
                    {
                        using T = std::decay_t<decltype(e)>;

                        if constexpr (std::same_as<T, DCS::KeyEvent>)
                        {
                            for (const auto element : ui_elements)
                                element->on_key_event(e);
                        }
                        else if constexpr (std::same_as<T, DCS::MouseEvent>)
                        {
                            const auto mouse_position = window->mouse_position();
                            for (const auto element : ui_elements)
                            {
                                if (mouse_position >= element->position())
                                    element->on_mouse_event(e, mouse_position - element->position());
                            }
                        }
                    },
                    *event);
                event = window->poll_event();
            }

            for (const auto element : ui_elements)
                element->update();

            renderer->clear();
            for (const auto element : ui_elements)
            {
                renderer->push_offset(element->position());
                element->draw();
                renderer->pop_offset();
            }

            window->update();
        }
    }
    catch (const DCS::Exception& e)
    {
        std::println(std::cerr, "Exception: {}", e);
        return 1;
    }
}
