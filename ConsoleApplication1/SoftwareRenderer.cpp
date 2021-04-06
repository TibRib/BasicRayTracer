#include "pch.h"
#include "SoftwareRenderer.h"

#include "utils.h"
#include <SDL.h>
#include <iostream>

constexpr static const int32_t g_kRenderDeviceFlags = -1;
constexpr static const int32_t g_kErrorOccurred = -1;

constexpr static const char* g_kWindowTitle = "PixelPusher";

int32_t e(int32_t result, std::string errorMessage)
{
    if (result)
        std::cout << errorMessage;

    return result;
}


// Free resources 
void Shutdown(SDL_Window** ppWindow, SDL_Renderer** ppRenderer, SDL_Texture** ppTexture)
{
    // Free the Back Buffer
    if (ppTexture)
    {
        SDL_DestroyTexture(*ppTexture);
        *ppTexture = nullptr;
    }

    // Free the SDL renderer
    if (ppRenderer)
    {
        SDL_DestroyRenderer(*ppRenderer);
        *ppRenderer = nullptr;
    }

    // Free the SDL window
    if (ppWindow)
    {
        SDL_DestroyWindow(*ppWindow);
        *ppWindow = nullptr;
    }
}

// Call this once during each render loop in order to determine when the user wishes to terminate the program
bool ProcessInput(){
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            return false;
    }
    return true;
}


SoftwareRenderer::SoftwareRenderer(int width, int height) {
    _width = width;
    _height = height;

    uint32_t wwidth = 1920;
    uint32_t wheight = 1080;

    /*Startup*/
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);
    // Get current device's Display Mode to calculate window position
    SDL_DisplayMode DM; SDL_GetCurrentDisplayMode(0, &DM);
    // Calculate where the upper-left corner of a centered window will be
    const int32_t x = DM.w / 2 - wwidth / 2;
    const int32_t y = DM.h / 2 - wheight / 2;

    /* Window creation */
    _pWindow = SDL_CreateWindow("RayTracer", x, y, wwidth, wheight, SDL_WINDOW_ALLOW_HIGHDPI);
    if (e(!_pWindow, "No Window. Aborting..."))
        Shutdown(&_pWindow, &_pRenderer, &_pTexture);
    
    _pRenderer = SDL_CreateRenderer(_pWindow, -1, SDL_RENDERER_SOFTWARE);
    if (e(!_pRenderer, "No Renderer. Aborting..."))
        Shutdown(&_pWindow, &_pRenderer, &_pTexture);

    _pTexture =  SDL_CreateTexture(_pRenderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, _width, _height);

    if (e(!_pTexture, "No back buffer Texture. Aborting..."))
        Shutdown(&_pWindow, &_pRenderer, &_pTexture);
    /*End startup*/

    std::cout << "Renderer initialized" << std::endl;
    std::cout << "Window (" << wwidth << ", " << wheight << " )" << std::endl;
    std::cout << "Render (" << _width << ", " << _height << " )" << std::endl;
}

SoftwareRenderer::SoftwareRenderer() : SoftwareRenderer(0, 0) {}

SoftwareRenderer::~SoftwareRenderer() {
    Shutdown(&_pWindow, &_pRenderer, &_pTexture);
    std::cout << "Renderer destroyed" << std::endl;
}

void SoftwareRenderer::writePixel(const int x, const int y, Color& cc) const {
    // Write the translated [0,255] value of each color component.
    int iR = static_cast<int>(256 * clamp(cc.r(), 0.0, 0.999));
    int iG = static_cast<int>(256 * clamp(cc.g(), 0.0, 0.999));
    int iB = static_cast<int>(256 * clamp(cc.b(), 0.0, 0.999));

    writePixel(x, y, iR, iG, iB);
}

void SoftwareRenderer::writePixel(const int x, const int y, int r, int g, int b) const {
}

void SoftwareRenderer::writeRowOfPixels(const int col, Color** row) const {
    /* Render */
    int32_t pitch = 0;
    uint32_t* pPixelBuffer = nullptr;
    // Lock the memory in order to write our Back Buffer image to it
    if (!SDL_LockTexture(_pTexture, NULL, (void**)&pPixelBuffer, &pitch))
    {
        // The pitch of the Back Buffer texture in VRAM must be divided by four bytes
        // as it will always be a multiple of four
        pitch /= sizeof(uint32_t);

        for (uint32_t i = 0; i < _width; ++i) {
            uint32_t iR = static_cast<uint32_t>(256 * clamp(row[i]->r(), 0.0, 0.999));
            uint32_t iG = static_cast<uint32_t>(256 * clamp(row[i]->g(), 0.0, 0.999));
            uint32_t iB = static_cast<uint32_t>(256 * clamp(row[i]->b(), 0.0, 0.999));

            uint32_t id = (uint32_t)col * (uint32_t)_width + (uint32_t)i;
           
            pPixelBuffer[id] = ARGB(iR, iG, iB, 255);
        }


        // Unlock the texture in VRAM to let the GPU know we are done writing to it
        SDL_UnlockTexture(_pTexture);

        // Copy our texture in VRAM to the display framebuffer in VRAM
        SDL_RenderCopy(_pRenderer, _pTexture, NULL, NULL);

        // Copy the VRAM framebuffer to the display
        SDL_RenderPresent(_pRenderer);

    }
}

void SoftwareRenderer::writeGridOfPixels(Color** grid, uint32_t width, uint32_t height) const {
    /* Render */
    int32_t pitch = 0;
    uint32_t* pPixelBuffer = nullptr;
    // Lock the memory in order to write our Back Buffer image to it
    if (!SDL_LockTexture(_pTexture, NULL, (void**)&pPixelBuffer, &pitch))
    {
        // The pitch of the Back Buffer texture in VRAM must be divided by four bytes
        // as it will always be a multiple of four
        pitch /= sizeof(uint32_t);

        for (uint32_t i = 0; i < width * height; ++i) {
            uint32_t iR = static_cast<uint32_t>(256 * clamp(grid[i]->r(), 0.0, 0.999));
            uint32_t iG = static_cast<uint32_t>(256 * clamp(grid[i]->g(), 0.0, 0.999));
            uint32_t iB = static_cast<uint32_t>(256 * clamp(grid[i]->b(), 0.0, 0.999));

            pPixelBuffer[i] = ARGB(iR, iG, iB, 255);
        }


        // Unlock the texture in VRAM to let the GPU know we are done writing to it
        SDL_UnlockTexture(_pTexture);

        // Copy our texture in VRAM to the display framebuffer in VRAM
        SDL_RenderCopy(_pRenderer, _pTexture, NULL, NULL);

        // Copy the VRAM framebuffer to the display
        SDL_RenderPresent(_pRenderer);

    }
}