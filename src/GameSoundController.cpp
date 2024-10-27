#include "GameSoundController.hpp"

GameSoundController::GameSoundController(GameData &data)
    : _data(data), _playerSoundController(_data.player), _ghostSoundController(data) {
  _startSample = Mix_LoadWAV("resources/sounds/start.wav");
  _eatGhostSample = Mix_LoadWAV("resources/sounds/eat_ghost.wav");
  if (_startSample == NULL || _eatGhostSample == NULL) {
    std::cerr << "Mix_LoadWAV error\n";
    exit(1);
  }
  Mix_Volume(-1, 10);
}

GameSoundController::~GameSoundController() {
  Mix_FreeChunk(_startSample);
  Mix_FreeChunk(_eatGhostSample);
}

void GameSoundController::play(Mix_Chunk *chunk, int loops) {
  Mix_HaltChannel(1);
  Mix_PlayChannel(1, chunk, loops);
  _playingNow = chunk;
}

void GameSoundController::update() {
  switch (_data.stage) {
  case GameData::Readiness:
    if (_data.lives == 3) {
#ifdef NDEBUG
      if (_playingNow != _startSample) {
        play(_startSample, 0);
      }
#endif
      return;
    }
    break;
  case GameData::GhostWhereEaten:
    if (_playingNow != _eatGhostSample) {
      play(_eatGhostSample, 0);
      _ghostSoundController.stop();
    }
    break;
  case GameData::MainGame:
    if (!Mix_Playing(1)) {
      _playingNow = NULL;
    }
    _playerSoundController.update();
    if (_data.player.state != PlayerData::Dying &&
        _data.player.state != PlayerData::Dead) {
      _ghostSoundController.update();
    } else {
      _ghostSoundController.stop();
    }
    break;
  default:
    break;
  }
}
