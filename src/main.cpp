#include <iostream>
#include <nlohmann/json.hpp>
#include <print>
#include <string_view>
#include <vector>

#include "Ensure.h"
#include "Grid.h"
#include "GridConnection.h"
#include "LogicGate.h"
#include "PowerSource.h"
#include "Renderer.h"
#include "Util.h"
#include "Window.h"
#include "WindowEvent.h"
#include "Wire.h"

int main(int argc, char** argv)
{
    try
    {
        DCS::ensure(argc == 2, "usage: {} <assets>", argv[0]);
        const auto assets_path = std::filesystem::path{argv[1]};

        const auto window = DCS::MakeRef<DCS::Window>(800, 600);
        const auto renderer = DCS::MakeRef<DCS::Renderer>(window, assets_path);

        const auto grid = DCS::MakeRef<DCS::Grid>();

        grid->add_object(DCS::MakeRef<DCS::LogicGate>(glm::ivec2{10, 10}, DCS::LogicGate::Type::AND));
        grid->add_object(DCS::MakeRef<DCS::LogicGate>(glm::ivec2{16, 11}, DCS::LogicGate::Type::XOR));
        grid->add_object(DCS::MakeRef<DCS::PowerSource>(glm::ivec2{5, 5}));

        // grid->add_wire(DCS::MakeRef<DCS::Wire>(
        //     std::vector{
        //         std::make_pair(glm::ivec2{5, 5}, glm::ivec2{5, 12}),
        //         std::make_pair(glm::ivec2{5, 10}, glm::ivec2{10, 10}),
        //         std::make_pair(glm::ivec2{5, 12}, glm::ivec2{10, 12})},
        //     std::vector{glm::ivec2{5, 5}, glm::ivec2{10, 10}, glm::ivec2{10, 12}}));

        // grid->add_wire(DCS::MakeRef<DCS::Wire>(
        //     std::vector{std::make_pair(glm::ivec2{13, 11}, glm::ivec2{16, 11})}, std::vector{glm::ivec2{13, 11}, glm::ivec2{16, 11}}));

        // grid->add_wire(
        //     DCS::MakeRef<DCS::Wire>(std::vector{std::make_pair(glm::ivec2{19, 12}, glm::ivec2{21, 12})}, std::vector{glm::ivec2{19,
        //     12}}));

        std::println("{}", grid->serialize().dump());

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
