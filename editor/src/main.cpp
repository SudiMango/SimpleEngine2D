#include <SDL2/SDL.h>
#include <string>
#include <SimpleEngine2D/core/ComponentRegistry.hpp>
#include <SimpleEngine2D/core/ConfigManager.hpp>
#include <SimpleEngine2D/core/Engine.hpp>
#include <SimpleEngine2D/core/EntityManager.hpp>
#include <SimpleEngine2D/core/PersistenceManager.hpp>
#include <SimpleEngine2D/core/TagManager.hpp>
#include "Hierarchy.hpp"
#include "Properties.hpp"
#include "TopBar.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "Viewport.hpp"

#if !SDL_VERSION_ATLEAST(2,0,17)
    #error This backend requires SDL 2.0.17+
#endif

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        return 1;
    }

    // Create window with SDL_Renderer graphics context
    float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("SimpleEngine2D Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)(1280 * main_scale), (int)(800 * main_scale), window_flags);
    if (window == nullptr) {
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (renderer == nullptr) {
        return 1;
    }

    // Setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    Uint64 lastTime = SDL_GetPerformanceCounter();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    simpleengine2d::core::ConfigManager::getInstance().getConfig().render.backgroundColor = {201, 145, 103, 255};
    auto &pm = simpleengine2d::core::PersistenceManager::getInstance();
    auto &em = simpleengine2d::core::EntityManager::getInstance();
    auto &tm = simpleengine2d::core::TagManager::getInstance();
    auto &cr = simpleengine2d::core::ComponentRegistry::getInstance();

    cr.registerAllComponents();

    simpleengine2d::core::EntityId selectedEntity = -1;

    simpleengine2d::editor::Viewport viewport(window, renderer);
    simpleengine2d::editor::TopBar topbar("GameScene");
    simpleengine2d::editor::Hierarchy hierarchy(selectedEntity);
    simpleengine2d::editor::Properties properties(selectedEntity);
    viewport.init();
    topbar.init();
    hierarchy.init();
    properties.init();

    // Main loop
    bool done = false;
    while (!done)
    {
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)(now - lastTime) / (float)SDL_GetPerformanceFrequency();
        lastTime = now;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        // --- Panels ---
        topbar.update(dt);
        viewport.update(dt);
        hierarchy.update(dt);
        properties.update(dt);

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }


    // Cleanup
    viewport.clean();
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
