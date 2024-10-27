#include "main.hpp"

void logSDLError(std::ostream &os, const std::string &message) {
  os << message << " " << SDL_GetError() << std::endl;
}

int sdlInit() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    logSDLError(std::cout, "SDL_Init error");
    return 1;
  }

  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
    logSDLError(std::cout, "IMG_init error");
    SDL_Quit();
    return 1;
  }

  if (TTF_Init() != 0) {
    logSDLError(std::cout, "IMG_Init error");
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 256) < 0) {
    logSDLError(std::cout, "Audio_Init error");
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  return 0;
}

void quitSdl() {
  IMG_Quit();
  TTF_Quit();
  Mix_CloseAudio();
  SDL_Quit();
}

int main(int argc, char **argv) {
  if (sdlInit()) {
    return 1;
  }

  SDL_Window *window =
      SDL_CreateWindow("pacman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    logSDLError(std::cout, "SDL_CreateWindow error");
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    logSDLError(std::cout, "SDL_CreateRenderer error");
    return 1;
  }

  Game game(renderer);

  SDL_Event event;
  bool quit = false;

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
      if (event.type == SDL_KEYDOWN) {
        game.keyDown(event.key);
      }
    }

    game.update();

    SDL_RenderClear(renderer);
    game.draw();
    SDL_RenderPresent(renderer);
  }

  game.writeHighScore();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  quitSdl();

  return 0;
}
