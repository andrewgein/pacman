#ifndef __GHOST_DRAWER_H__
#define __GHOST_DRAWER_H__

#include "GameData.hpp"
#include "GhostController.hpp"
#include "Global.hpp"
#include "Texture.hpp"
#include "Timer.hpp"

class GhostDrawer : public Drawable {
public:
  GhostDrawer(const GhostData &, SDL_Renderer *, const point_t &, const point_t &);
  void draw() const;
  void updateFrame();
#ifndef NDEBUG
  void drawPath() const;
#endif

protected:
  SDL_Renderer *_renderer;
  Texture *_ghostTextures[4][2];
  Texture *_frightenedTextures[2];
  Texture *_deadTextures[4];
#ifndef NDEBUG
  Texture *_pointTexture;
  Texture *_positionTexture;
  Texture *_cellPositionTexture;
#endif
  short _currentFrame;
  Timer _animationTimer;
  const GhostData &_data;
  static const short FRAME_COUNT = 2;
  static const short FRAME_STEP = 6;
  static const short WIDTH = 40;
  static const short HEIGHT = 40;
  static const short START_FRIGHTENED_FRAME_X = 1048;
  static const short START_FRIGHTENED_FRAME_Y = 180;
  static const short START_DEAD_CLIP_X = 1050;
  static const short START_DEAD_CLIP_Y = 220;
};

#endif
