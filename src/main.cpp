#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <print>
#include <string_view>

#include "Ensure.h"
#include "Grid.h"
#include "GridConnection.h"
#include "Renderer.h"
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

        const auto grid = DCS::MakeRef<DCS::Grid>();

        std::ifstream f(assets_path / "test_scene.json");
        grid->deserialize(nlohmann::json::parse(f));

        while (!window->should_close())
        {
            auto event = window->poll_event();
            while (event)
            {
                grid->process_event(*event, window);
                event = window->poll_event();
            }

            grid->update();

            renderer->clear();
            grid->draw(renderer);

            window->update();
        }
    }
    catch (const DCS::Exception& e)
    {
        std::println(std::cerr, "Exception: {}", e);
        return 1;
    }
}
