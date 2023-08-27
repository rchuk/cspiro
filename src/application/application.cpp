#include "application.hpp"

#include <cstdint>
#include <iostream>

#include "GL/glew.h"
#include "GL/GL.h"


Application::~Application() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Application::run()  {
    if (!init())
        return false;

    on_start();
    loop();

    return true;
}

bool Application::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        std::cout << "Couldn't initialize SDL2: " << SDL_GetError();

        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::cout << "Couldn't create window: " << SDL_GetError();

        return false;
    }

    glewExperimental = GL_TRUE;
    const SDL_GLContext gl = SDL_GL_CreateContext(window);

    if (!gl) {
        std::cout << "Couldn't create OpenGL context: " << SDL_GetError();

        return false;
    }

    if (GLuint result = glewInit(); result != GLEW_OK) {
        std::cout << "Couldn't initialize GLEW: " << glewGetErrorString(result);

        return false;
    }

    on_resize();

    return true;
}

void Application::loop() {
    uint64_t old_time = SDL_GetTicks64();

    bool is_running = true;
    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;

                break;
            } else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    width = event.window.data1;
                    height = event.window.data2;
                    
                    on_resize();
                }
            }
            
            on_event(event);
        }

        const uint64_t time = SDL_GetTicks64();
        const uint64_t delta_time = time - old_time;
        old_time = time; 

        on_update(delta_time / 1000.0f);
        render();
    }
}

void Application::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    on_render();

    SDL_GL_SwapWindow(window);
}

void Application::on_resize() {
    glViewport(0, 0, width, height);

    camera.on_viewport_change(glm::vec2(width, height));
}
