#include <stdlib.h>
#include <SDL2/SDL.h>

#define internal static
#define local_persist static
#define global_variable static

typedef struct
{
    SDL_Window *Window;
    SDL_Renderer *Renderer;
} SDL_WinRender;

// TODO: This is a global for now
global_variable bool Running;
global_variable SDL_Texture *Texture;
global_variable void *Pixels;
global_variable int TextureWidth;

internal SDL_WinRender
GetSDL_WinRender(SDL_Event *Event)
{
    SDL_Window *Window = SDL_GetWindowFromID(Event->window.windowID);
    SDL_Renderer *Renderer = SDL_GetRenderer(Window);

    return SDL_WinRender
    {
        Window,
        Renderer
    };
}

internal void
SDLResizeTexture(SDL_Renderer *Renderer, int Width, int Height)
{
    if (Pixels)
    {
        free(Pixels);
    }
    if (Texture)
    {
        SDL_DestroyTexture(Texture);
    }

    Texture = SDL_CreateTexture(Renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                Width,
                                Height);
    TextureWidth = Width;
    Pixels = malloc(Width * Height * 4);
}

internal void
SDLUpdateWindow(SDL_Renderer *Renderer)
{
    if (Texture) {
        SDL_UpdateTexture(Texture,
                          0,
                          Pixels,
                          TextureWidth * 4);

        SDL_RenderCopy(Renderer,
                       Texture,
                       0,
                       0);
    }
    else {
        // TODO: There is no texture!
        SDL_LogError(0, "There is no texture!");
    }

    SDL_RenderPresent(Renderer);
}

internal void
HandleEvent(SDL_Event *Event)
{
    switch(Event->type)
    {
        case SDL_QUIT:
        {
            SDL_LogDebug(0, "SDL_QUIT");
            Running = false;
        } break;

        case SDL_WINDOWEVENT:
        {
            switch(Event->window.event)
            {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
                    SDL_LogDebug(0, "SDL_WINDOWEVENT_SIZE_CHANGED (%d, %d)", Event->window.data1, Event->window.data2);

                    SDL_WinRender WinRend = GetSDL_WinRender(Event);
                    SDLResizeTexture(WinRend.Renderer, Event->window.data1, Event->window.data2);
                } break;

                case SDL_WINDOWEVENT_FOCUS_GAINED:
                {
                    SDL_LogDebug(0, "SDL_WINDOWEVENT_FOCUS_GAINED");
                } break;

                case SDL_WINDOWEVENT_EXPOSED:
                {
                    SDL_LogDebug(0, "SDL_WINDOWEVENT_EXPOSED");

                    SDL_WinRender WinRend = GetSDL_WinRender(Event);
                    SDLUpdateWindow(WinRend.Renderer);
                } break;
            }
        } break;
    }
}

int
main(int argc, char *argv[])
{
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    SDL_Window *Window;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        // TODO: SDL_Init didn't work!
        SDL_LogError(0, "Unable to initialize SDL: %s", SDL_GetError());
    }

    SDL_LogDebug(0, "Starting Bomber Hero!");

    Window = SDL_CreateWindow("Bomber Hero",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              640,
                              480,
                              SDL_WINDOW_RESIZABLE);

    if (Window)
    {
        SDL_Renderer *Renderer = SDL_CreateRenderer(Window, -1, 0);

        if (Renderer)
        {
            Running = true;
            while (Running) {
                SDL_Event Event;
                SDL_WaitEvent(&Event);
                HandleEvent(&Event);
            }
        }
        else
        {
            // TODO: Unable to create Renderer!
        }
    }
    else
    {
        // TODO: Unable to create Window!
    }

    SDL_Quit();

    return 0;
}
