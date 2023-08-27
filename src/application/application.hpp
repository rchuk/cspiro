#pragma once

#include <string>

#include "SDL2/SDL.h"

#include "graphics/camera.hpp"


class Application {
public:
    Application(std::string title, int width, int height)
        : title(title), width(width), height(height) {}

    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;

    ~Application();

    bool run();

private:
    SDL_Window* window = nullptr;
    const std::string title;
    int width;
    int height;

    Camera camera;

private:
    bool init();

    void loop();
    void render();

    void on_resize();

protected:
    Camera& get_camera() {
        return camera;
    }

    virtual void on_start() {}
    virtual void on_event(const SDL_Event& event) { static_cast<void>(event); }
    virtual void on_update(float dt) { static_cast<void>(dt); }
    virtual void on_render() {}
};
