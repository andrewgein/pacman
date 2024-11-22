#include "PlayerSoundController.hpp"

PlayerSoundController::PlayerSoundController(PlayerData &data) : _data(data) {
  _playingNow = NULL;
  _wakaSample1 = Mix_LoadWAV("resources/sounds/eat_dot_0.wav");
  _wakaSample2 = Mix_LoadWAV("resources/sounds/eat_dot_1.wav");
  _deathSample = Mix_LoadWAV("resources/sounds/death_2.wav");
  if (_wakaSample1 == NULL || _wakaSample2 == NULL || _deathSample == NULL) {
    std::cerr << "Mix_LoadWAV error\n";
    exit(1);
  }
  _wakaSamplesFlag = true;
}

PlayerSoundController::~PlayerSoundController() {
  Mix_FreeChunk(_wakaSample1);
  Mix_FreeChunk(_wakaSample2);
  Mix_FreeChunk(_deathSample);
}

void PlayerSoundController::play(Mix_Chunk *chunk, int loops) {
  Mix_HaltChannel(2);
  Mix_PlayChannel(2, chunk, loops);
  _playingNow = chunk;
}

void PlayerSoundController::update() {
  if (_data.isEating == true && !Mix_Playing(2)) {
    if (_wakaSamplesFlag) {
      play(_wakaSample1, 0);
    } else {
      play(_wakaSample2, 0);
    }
    _wakaSamplesFlag = !_wakaSamplesFlag;
  }
  if (_data.state == PlayerData::Dying && _playingNow != _deathSample) {
    play(_deathSample, 0);
  }
  if (!Mix_Playing(2)) {
    _playingNow = NULL;
  }
}
