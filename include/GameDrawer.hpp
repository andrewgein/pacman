#ifndef __GAME_DRAWER_H__
#define __GAME_DRAWER_H__

#include "BlinkyDrawer.hpp"
#include "ClydeDrawer.hpp"
#include "Drawable.hpp"
#include "GameData.hpp"
#include "GameInterface.hpp"
#include "Global.hpp"
#include "InkyDrawer.hpp"
#include "Map.hpp"
#include "PinkyDrawer.hpp"
#include "PlayerDrawer.hpp"

class GameDrawer : public Drawable {
public:
  GameDrawer(const GameData &, const Map &, SDL_Renderer *);
  ~GameDrawer();
  void draw() const;

private:
  void drawLives() const;
#ifndef NDEBUG
  void drawTiles() const;
#endif
  const GameData *_data;
  SDL_Renderer *_renderer;
  const Map &_map;
  const point_t MAP_OFFSET = {MAP_OFFSET_X, MAP_OFFSET_Y};
  PlayerDrawer _playerGraphics;
  std::array<GhostDrawer *, 4> _ghosts;
  GameInterface _gameInterface;
  Texture _gameOverTitle;
  Texture _liveTexture;
  Texture _readyTitle;
  Texture _points[4];
};

#endif
