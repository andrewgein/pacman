#ifndef __GHOST_SOUND_CONTROLLER_H__
#define __GHOST_SOUND_CONTROLLER_H__

#include <SDL_mixer.h>
#include <array>
#include <iostream>

#include "GameData.hpp"

class GhostSoundController {
public:
  GhostSoundController(GameData &);
  ~GhostSoundController();
  void update();
  void stop();

private:
  void play(Mix_Chunk *chunk, int loops);
  Mix_Chunk *_sirenSample, *_frightSample, *_eyesSample;
  Mix_Chunk *_playingNow;
  std::array<GhostData *, 4> _ghosts;
};

#endif
