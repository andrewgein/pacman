#ifndef __PLAYER_SOUND_CONTROLLER_H__
#define __PLAYER_SOUND_CONTROLLER_H__

#include <SDL_mixer.h>
#include <iostream>

#include "GameData.hpp"

class PlayerSoundController {
public:
  PlayerSoundController(PlayerData &);
  ~PlayerSoundController();
  void update();

private:
  void play(Mix_Chunk *chunk, int loops);
  Mix_Chunk *_wakaSample1, *_wakaSample2, *_deathSample;
  Mix_Chunk *_playingNow;
  PlayerData &_data;
  bool _wakaSamplesFlag;
};

#endif
