#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include "asset_path.h"
#include "cleanup.h"
#include "accel.h"

// Window dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FRAME_DUR = 0; // in millis

/**
 * Log an SDL error with some error message to an output stream
 * @param os The output stream to write the message to
 * @param msg The error message to write; format will be `msg error: SDL_GetError()`
 */
void logSDLError(std::ostream &os, const std::string &msg) {
  os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
 * Loads a BMP image into a texture on the rendering device
 * @param file The BMP image file to load
 * @param ren The renderer to load the texture onto
 * @return The loaded texture, or nullptr if something went wrong.
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

  // Set some GL attributes
  // TODO Are these really the settings I want? Correct major and minor versions, etc?
  // TODO can I use macros here to make this less verbose?
  if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)) {
    logSDLError(std::cout, "SDL_GL_SetAttribute");
  }
  if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3)) {
    logSDLError(std::cout, "SDL_GL_SetAttribute");
  }
  if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2)) {
    logSDLError(std::cout, "SDL_GL_SetAttribute");
  }
  if (SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8)) {
    logSDLError(std::cout, "SDL_GL_SetAttribute");
  }

  // Get the resolution
  SDL_DisplayMode dm;
  if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
    logSDLError(std::cout, "SDL_GetCurrentDisplayMode");
    return 1;
  }

  // Make a window
  SDL_Window *win = SDL_CreateWindow("Hello world!", 100, 100, dm.w, dm.h,
                                     SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  if (win == nullptr) {
    logSDLError(std::cout, "SDL_CreateWindow");
    SDL_Quit();
    return 1;
  }

#ifndef DEBUG
  // Make the window fullscreen. Feature-flaged for release only.
  if (SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN)) {
      logSDLError(std::cout, "SDL_SetWindowFullscreen");
    }
#endif

// Temporarily feature-flagged
# ifdef DEBUG
  // Make a GL context
  SDL_GLContext context = SDL_GL_CreateContext(win);
  if (context == nullptr) {
    logSDLError(std::cout, "SDL_GL_CreateContext");
    cleanup(win);
    SDL_Quit();
    return 1;
  }
#else
  void* context = nullptr;
#endif

  // Make a renderer, to draw to the window
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
               SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr) {
    logSDLError(std::cout, "SDL_CreateRenderer");
    cleanup(win, &context);
    SDL_Quit();
    return 1;
  }

  // Draw the texture on the screen. Here's a rendering loop.
  SDL_Event e;
  bool quit = false;
  Uint8 r_channel = 0;
  Uint8 g_channel = 0;
  Uint8 b_channel = 0;
  int accel_x_raw = 0;
  int accel_y_raw = 0;
  int accel_z_raw = 0;
  while (!quit) {
    // Print the acceleration!
    accel_x_raw = in_accel_x_raw();
    accel_y_raw = in_accel_y_raw();
    accel_z_raw = in_accel_z_raw();

    std::cout << accel_x_raw << ","
              << accel_y_raw << ","
              << accel_z_raw << ";    "
              << (int)scale_accel(accel_x_raw) << ","
              << (int)scale_accel(accel_y_raw) << ","
              << (int)scale_accel(accel_z_raw) << ","
              << std::endl;

    while (SDL_PollEvent(&e)) {
      // User closes the window
      if (e.type == SDL_QUIT) {
        quit = true;
        std::cout << "Quit!" << std::endl;
      }

      // User presses a key
      if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_q) {
          quit = true;
        }
      }

#ifndef DEBUG
      // User touches touchscreen
      if (e.type == SDL_FINGERDOWN) {
        std::cout << "Touched!" << std::endl;
      }
#endif
    }

    SDL_RenderClear(ren);
    // Draw the background
    // SDL_SetRenderDrawColor(ren, 0, 0, brightness, 0);
    r_channel = scale_accel(accel_x_raw);
    g_channel = scale_accel(accel_y_raw);
    b_channel = scale_accel(accel_z_raw);
    SDL_SetRenderDrawColor(ren, r_channel, g_channel, b_channel, 0);
    // Update the screen
    SDL_RenderPresent(ren);
    // Idle for a little while
    SDL_Delay(FRAME_DUR);
  }

  // Finally, clean everything up.
  cleanup(ren, win, &context);
  SDL_Quit();
}
