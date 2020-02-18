#ifndef CLEANUP_H
#define CLEANUP_H

#include <utility>
#include <SDL2/SDL.h>

/*
 * Recurse through the list of arguments to clean up, cleaning up the first one
 * in the list in each iteration.
 */
template<typename T, typename... Args>
void cleanup(T *t, Args&&... args) {
  // Clean up the first item in the list
  cleanup(t);
  // Clean up the rest
  cleanup(std::forward<Args>(args)...);
}

/*
 * Specializations to provide base cases to the recursive call above
 */
template<>
inline void cleanup<SDL_Window>(SDL_Window *win) {
  if (!win) {
    return;
  }
  SDL_DestroyWindow(win);
}

template<>
inline void cleanup<SDL_Renderer>(SDL_Renderer *ren) {
  if (!ren) {
    return;
  }
  SDL_DestroyRenderer(ren);
}

template<>
inline void cleanup<SDL_Texture>(SDL_Texture *tex) {
  if (!tex) {
    return;
  }
  SDL_DestroyTexture(tex);
}

template<>
inline void cleanup<SDL_Surface>(SDL_Surface *surf) {
  if (!surf) {
    return;
  }
  SDL_FreeSurface(surf);
}

#endif
