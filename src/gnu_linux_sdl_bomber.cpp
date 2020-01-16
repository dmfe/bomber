#include <SDL2/SDL.h>

bool HandleEvent(SDL_Event *Event) {
    bool ShouldQuit = false;

    switch(Event->type) {
        case SDL_WINDOWEVENT: {
            switch(Event->window.event) {
                case SDL_WINDOWEVENT_RESIZED: {
                    SDL_Log("Window Resize(%d, %d)", Event->window.data1, Event->window.data2);
                    break;
                }
                case SDL_WINDOWEVENT_EXPOSED: {
                    SDL_Window *Window = SDL_GetWindowFromID(Event->window.windowID);
                    SDL_Renderer *Renderer = SDL_GetRenderer(Window);
                    static bool IsWhite = true;

                    if (IsWhite) {
                        SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
                        IsWhite = false;
                    } else {
                        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
                        IsWhite = true;
                    }

                    SDL_RenderClear(Renderer);
                    SDL_RenderPresent(Renderer);

                    break;
                }
            }
            break;
        }
        case SDL_QUIT: {
            SDL_Log("Exiting Bomber Hero...");
            ShouldQuit = true;
            break;
        }
    }

    return (ShouldQuit);
}

int main(int argc, char *argv[]) {

    SDL_Window *Window;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // TODO: SDL_Init didn't work!
        SDL_LogError(0, "Unable to initialize SDL: %s", SDL_GetError());
    }

    SDL_LogInfo(0, "This is Bomber Hero!");

    Window = SDL_CreateWindow("Bomber Hero",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              640,
                              480,
                              SDL_WINDOW_RESIZABLE);

    if (Window) {
        SDL_Renderer *Renderer = SDL_CreateRenderer(Window, -1, 0);
        if (Renderer) {
            for (;;) {
                SDL_Event Event;
                SDL_WaitEvent(&Event);
                if (HandleEvent(&Event)) {
                    break;
                }
            }
        } else {
            //TODO: Unable to create Renderer!
        }
    } else {
        //TODO: Unable to create Window!
    }

    SDL_Quit();

    return 0;
}
