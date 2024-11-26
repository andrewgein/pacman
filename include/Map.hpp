#ifndef __MAP_H__
#define __MAP_H__

#include <map>
#include <queue>
#include <vector>

#include "Drawable.hpp"
#include "Global.hpp"
#include "Point.h"
#include "Texture.hpp"
#include "Timer.hpp"
#include <SDL2/SDL.h>

class Map : public Drawable {
public:
  Map(SDL_Renderer *);
  void draw() const;
  void drawFood() const;
  void startBlinking();
  void stopBlinking();
  bool wallCollision(const point_t &, const short &) const;
  bool wallCollision(const cpoint_t &, const short &) const;
  short foodCollision(const point_t &);
  const point_t getTileCenter(const point_t &);
  std::vector<cpoint_t> buildPathWithDirection(const Direction &, const cpoint_t &,
                                               const cpoint_t &) const;
  std::vector<cpoint_t> buildPath(const cpoint_t &, const cpoint_t &) const;
  bool isExactLocation(const point_t &, const cpoint_t &) const;

  bool isInTileCenter(const point_t &) const;
  bool isOnTileEdge(const point_t &) const;
  bool isInTeleport(const point_t &) const;
  static bool isColliding(const point_t &, const point_t &);
  void restart();
  bool areAnyDotsLeft() const;

private:
  Texture _mapTexture;
  Texture _smallFoodTexture;
  Texture _bigFoodTexture;
  SDL_Renderer *_renderer;

  char **mapSchematic;

  const point_t DOOR_POSITION_R = {14, 12};
  const point_t DOOR_POSITION_L = {13, 12};
  int _dotsLeft;
  static const int START_DOTS_COUNT = 244;
  Timer _blinkTimer;
  bool _isVisible;
  bool _isBlinking;
  static const int BLINK_TICKS = 200;
};

#endif
