#ifndef ASSET_PATH_H
#define ASSET_PATH_H

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

std::string getAssetPath(const std::string &asset = "") {
#ifdef _WIN32
  const char PATH_SEP = '\\';
#else
  const char PATH_SEP = '/';
#endif
  // This call is pretty slow, so we'll juse a static lifetime here.
  static std::string baseRes;
  if (baseRes.empty()) {
    char *basePath = SDL_GetBasePath();
    if (basePath) {
      baseRes = basePath;
      SDL_free(basePath);
    } else {
      std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
      return "";
    }

    // Replace up to build/ with assets/
    // This should one day, probably, copy the assets into build/apps/assets
    // and do a `baseRes.rfind("bin");` instead.
    size_t pos = baseRes.rfind("build");
    baseRes = baseRes.substr(0, pos) + "assets" + PATH_SEP;
  }

  return asset.empty() ? baseRes : baseRes + asset;
}

#endif
