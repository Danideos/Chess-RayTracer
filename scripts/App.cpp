#include "App.h"
#include "Log.h"
#include "RayTrace/Scene.h"
#include "Random/Random.h"

App::App() {
    running_ = false;
    windowWidth_ = WINDOW_WIDTH;
    windowHeight_ = WINDOW_HEIGHT;
}

int App::Execute() {

    running_ = true;
    if (!Init()) {
        return 1;
    }

    SDL_Event event;
    while (running_){
        while(SDL_PollEvent(&event) != 0){
            Event(&event);
            if (event.type == SDL_QUIT) {
                running_ = false;
            }
        }
        // Currently unused, preparing everything during init
        Loop();
        Render();
    }

    Cleanup();

    return 0;
}

bool App::Init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;

    if((pWindow_ = SDL_CreateWindow("RealChess RayTracer",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            windowWidth_, windowHeight_, SDL_WINDOW_SHOWN)) == NULL) return false;

    if ((pRenderer_ = SDL_CreateRenderer(pWindow_, -1,
                                        SDL_RENDERER_ACCELERATED)) == NULL) return false;

    // SDL_SetRenderDrawColor(pRenderer, 0x00, 0x00, 0x00, 0xFF);
    scene_.Initialize(windowWidth_, windowHeight_, pRenderer_);

    SDL_RenderClear(pRenderer_);
    scene_.Render();
    scene_.Display();
    SDL_RenderPresent(pRenderer_);

    return true;
}

void App::Render() {
    // Currently the rendering happens only once during initialization
    SDL_RenderClear(pRenderer_);
    scene_.Display();
    SDL_RenderPresent(pRenderer_);
}

void App::Loop() {
    // Nothing to loop through currently
}

void App::Cleanup() {
    if(pRenderer_) {
        SDL_DestroyRenderer(pRenderer_);
        pRenderer_ = NULL;
    }
    if(pWindow_) {
        SDL_DestroyWindow(pWindow_);
        pWindow_ = NULL;
    }
    SDL_Quit();
}

void App::Event(SDL_Event *event) {

}