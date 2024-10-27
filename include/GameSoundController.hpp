#ifndef __GAME_SOUND_CONTROLLER_H__
#define __GAME_SOUND_CONTROLLER_H__

#include <SDL_mixer.h>

#include "GameData.hpp"
#include "GhostSoundController.hpp"
#include "PlayerSoundController.hpp"

class GameSoundController {
public:
  GameSoundController(GameData &);
  ~GameSoundController();
  void update();

private:
  void play(Mix_Chunk *, int);
  Mix_Chunk *_startSample, *_eatGhostSample;
  Mix_Chunk *_playingNow;

  GameData &_data;
  PlayerSoundController _playerSoundController;
  GhostSoundController _ghostSoundController;
};

#endif
