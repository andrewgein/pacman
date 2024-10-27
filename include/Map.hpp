#ifndef __MAP_H__
#define __MAP_H__

#include <map>
#include <queue>
#include <vector>

#include "Drawable.hpp"
#include "Global.hpp"
#include "Point.h"
#include "Texture.hpp"
#include <SDL2/SDL.h>

class Map : public Drawable {
public:
  Map(SDL_Renderer *);
  void draw() const;
  void drawFood() const;
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

private:
  Texture _mapTexture;
  Texture _smallFoodTexture;
  Texture _bigFoodTexture;
  SDL_Renderer *_renderer;

  char **mapSchematic;

  const point_t DOOR_POSITION_R = {14, 12};
  const point_t DOOR_POSITION_L = {13, 12};
};

#endif
