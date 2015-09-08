#include <Graphics/Window.h>

namespace LSEngine
{
    Window::Window(const std::string& title, uint32_t width, uint32_t height, bool border)
    : Title(title), Width(width), Height(height), Border(border)
    { Create(); }

    Window::~Window() 
    { Destroy(); }

    void Window::LoadSettings()
    {
        //SETTINGS.Load();
        if (Border)
        {
            FullScreen = false;
            SDLFlags = SDL_WINDOW_BORDERLESS;
        }
        else
        {
            //FullScreen = SETTINGS.GetFullscreen();
            SDLFlags = FullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN;
            //Width = SETTINGS.GetWidth();
            //Height = SETTINGS.GetHeight();
        }
        //Antialiasing = SETTINGS.GetAA();
        SDLRenFlags = Antialiasing ? SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC : SDL_RENDERER_ACCELERATED;
    }

    void Window::Create()
    {
        LoadSettings();
        auto log = spdlog::get("Somnia");
        SDLWindow = SDL_CreateWindow(
            Title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            Width,
            Height,
            SDLFlags
            );
        if (!SDLWindow) { log->critical(SDL_GetError()); }

        SDLRenderer = SDL_CreateRenderer(SDLWindow, -1, SDLRenFlags);
        if (!SDLRenderer) { log->critical(SDL_GetError()); }

        LSEngine::Color bg = LSEngine::Color(0x7F141F);
        SDL_SetRenderDrawColor(SDLRenderer, bg.R(), bg.G(), bg.B(), bg.A());
        SDL_RenderSetLogicalSize(SDLRenderer, Width, Height);

        SetIcon();
    }

    void Window::Destroy()
    {
        if (SDLRenderer)
        {
            SDL_DestroyRenderer(SDLRenderer);
            SDLRenderer = nullptr;
        }
        if (SDLWindow)
        {
            SDL_DestroyWindow(SDLWindow);
            SDLWindow = nullptr;
        }
    }

    void Window::SetFullScreen(bool val)
    {
        if (FullScreen == val) return;
        Destroy();
        //SETTINGS.SetFullscreen(val);
        Create();
    }

    void Window::SetAntiAliasing(bool val)
    {
        if (Antialiasing == val) return;
        Destroy();
        //SETTINGS.SetAA(val);
        Create();
    }

    void Window::SetIcon(const std::string & file)
    {
        SDL_Surface* ico = IMG_Load(file.c_str());
        if (!ico) 
        {
            auto log = spdlog::get("Somnia");
            log->error(IMG_GetError());
        }

        SDL_SetWindowIcon(SDLWindow, ico);
        SDL_FreeSurface(ico);
    }
}