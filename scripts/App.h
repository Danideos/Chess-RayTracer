#ifndef MAIN_CPP_APP_H
#define MAIN_CPP_APP_H

#include <SDL.h>
#include "RayTrace/Scene.h"

constexpr const int WINDOW_WIDTH = 1280;
constexpr const int WINDOW_HEIGHT = 720;

class App{
public:
    App();

    int Execute();

private:
    // SDL2 stuff
    SDL_Window* pWindow_ = NULL;
    SDL_Renderer* pRenderer_ = NULL;

    // Scene parameters
    RT::Scene scene_;
    size_t windowWidth_;
    size_t windowHeight_;

    bool running_;
private:
    bool Init();
    void Event(SDL_Event* event);
    void Render();
    void Loop();
    void Cleanup();
};

#endif