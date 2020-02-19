#include <SDL2/SDL.h>
#include <iostream>
#include "asset_path.h"
#include "cleanup.h"

// Window dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FRAME_DUR = 40; // in millis


/**
 * Log an SDL error with some error message to an output stream
 * @param os The output stream to write the message to
 * @ param msg The error message to write; format will be `msg error: SDL_GetError()`
 */
void logSDLError(std::ostream &os, const std::string &msg) {
  os << msg << " error: " << SDL_GetError() << std::endl;
}


/**
 * Loads a BMP image into a texture on the rendering device
 * @param file The BMP image file to load
 * @param ren The renderer to load the texture onto
 * @rreturn The loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren) {
  // Initialize to nullptr to avoid dangling pointer issues
  SDL_Texture *texture = nullptr;
  // Load the image
  SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
  // If the loading succeeded, convert to texture and return the texture
  if (loadedImage != nullptr) {
    texture = SDL_CreateTextureFromSurface(ren, loadedImage);
    cleanup(loadedImage);
    // Check error condition
    if (texture == nullptr) {
      logSDLError(std::cout, "CreateTextureFromSurface");
    }
  } else {
    logSDLError(std::cout, "LoadBMP");
  }
  return texture;
}


/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param texThe source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
  // Set the destionation rectangle to be at the position we want
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  // Query the texture to get its width and heigh
  SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
  SDL_RenderCopy(ren, tex,NULL, &dst);
}


/**
 * main function that does stuff
 */
int main(int argc, char* args[]) {
  // Initialize SDL2
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    logSDLError(std::cout, "SDL_Init");
    return 1;
  }

  // Get the resolution
  SDL_DisplayMode dm;
  if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
    logSDLError(std::cout, "SDL_GetCurrentDisplayMode");
    return 1;
  }

  // Make a window
  SDL_Window *win = SDL_CreateWindow("Hello world!", 100, 100,
                                     dm.w, dm.h, SDL_WINDOW_SHOWN);
  if (win == nullptr) {
    logSDLError(std::cout, "SDL_CreateWindow");
    SDL_Quit();
    return 1;
  }

  // Make the window fullscreen
  if (SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN) < 0) {
    logSDLError(std::cout, "SDL_SetWindowFullscreen");
  }

  // Make a renderer, to draw to the window
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
               SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr) {
    logSDLError(std::cout, "SDL_CreateRenderer");
    cleanup(win);
    SDL_Quit();
    return 1;
  }

  // Draw the texture on the screen. Here's a rendering loop.
  SDL_Event e;
  bool quit = false;
  Uint8 brightness = 0;
  const Uint8 brightness_inc = 10;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      // User closes the window
      if (e.type == SDL_QUIT) {
        quit = true;
        std::cout << "Quit!" << std::endl;
      }

      // User presses a key
      if (e.type == SDL_KEYDOWN) {
        switch(e.key.keysym.sym) {
          case SDLK_q:
            quit = true;
            break;
          case SDLK_UP:
            brightness = brightness >= UINT8_MAX - brightness_inc ?
              UINT8_MAX : brightness + brightness_inc;
            break;
          case SDLK_DOWN:
            brightness = brightness <= brightness_inc ?
              0 : brightness - brightness_inc;
            break;
        }
      }

      // User touches touchscreen
      if (e.type == SDL_FINGERDOWN) {
        std::cout << "Touched!" << std::endl;

        // Increment the brightness, depending on position
        if (e.tfinger.y > 0.5) {
          brightness = brightness >= UINT8_MAX - brightness_inc ?
            UINT8_MAX : brightness + brightness_inc;
        } else {
          brightness = brightness <= brightness_inc ?
            0 : brightness - brightness_inc;
        }
      }

      // User clicks the mouse
      if (e.type == SDL_MOUSEBUTTONDOWN) {
        std::cout << "Mouse!" << std::endl;
      }
    }

    SDL_RenderClear(ren);
    // Draw the background
    SDL_SetRenderDrawColor(ren, 0, 0, brightness, 0);
    // Update the screen
    SDL_RenderPresent(ren);
    // Idle for a little while
    SDL_Delay(FRAME_DUR);
  }

  // Finally, clean everything up.
  cleanup(ren, win);
  SDL_Quit();
}
