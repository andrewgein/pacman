#ifndef __GAME_H__
#define __GAME_H__

#include <fstream>

#include <SDL2/SDL.h>

#include "GameController.hpp"
#include "GameDrawer.hpp"
#include "IntroController.hpp"
#include "IntroDrawer.hpp"
#include "Map.hpp"

class Game {
public:
  Game(SDL_Renderer *);
  void draw();
  void update();
  void keyUp(SDL_KeyboardEvent &);
  void keyDown(SDL_KeyboardEvent &);
  void writeHighScore();

private:
  void setHighScore();
  SDL_Renderer *_renderer;
  Map _map;
  GameData _data;
  IntroController _introController;
  IntroDrawer _introDrawer;
  GameController _gameController;
  GameDrawer _gameDrawer;

  static const int RESTART_DELAY = 200;
};

#endif
