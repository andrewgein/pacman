#ifndef __PLAYER_DRAWER_H__
#define __PLAYER_DRAWER_H__

#include <SDL2/SDL.h>

#include "Drawable.hpp"
#include "GameData.hpp"
#include "Texture.hpp"
#include "Timer.hpp"

class PlayerDrawer : public Drawable {
public:
  PlayerDrawer(const PlayerData &, SDL_Renderer *, const short &offsetX,
               const short &offsetY);
  void draw() const;
  bool isDeathAnimationEnd();
  void updateFrame();

private:
  void drawPointer() const;
  SDL_Renderer *_renderer;
  Texture *_playerTextures[4][3];
  Texture *_playerPointerTextures[4];
  Texture *_deadAnimationTextures[12];
  const PlayerData &_data;
  static const short HEIGHT = 36;
  static const short WIDTH = 36;
  static const short FRAME_STEP = 8;
  static const short FRAME_COUNT = 3;
  static const short DEAD_FRAME_COUNT = 12;
  static const short DEAD_FRAME_STEP = 3;
  static const short POINTER_HEIGHT = 12;
  static const short POINTER_WIDTH = 6;
  static const short ANIMATION_TICKS = 100;
  static const short START_CLIP_X = 699;
  static const short START_CLIP_Y = 4;
  static const short START_DEAD_CLIP_X = 787;
  static const short START_DEAD_CLIP_Y = 3;
  static const short START_POINTER_CLIP_X = 680;
  static const short START_POINTER_CLIP_Y = 0;
  static const short POINTER_CLIP_STEP = 16;
  short _deadFrame;
  short _currentFrame;
  short _isInRewind;
  Timer _animationTimer;
};

#endif
