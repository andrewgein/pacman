#include "GhostSoundController.hpp"

GhostSoundController::GhostSoundController(GameData &data) {
  _sirenSample = Mix_LoadWAV("resources/sounds/siren0.wav");
  _frightSample = Mix_LoadWAV("resources/sounds/fright_firstloop.wav");
  _eyesSample = Mix_LoadWAV("resources/sounds/eyes.wav");
  if (_sirenSample == NULL || _frightSample == NULL || _eyesSample == NULL) {
    std::cerr << "Mix_LoadWAV error\n";
    exit(1);
  }
  _ghosts[0] = &data.blinky;
  _ghosts[1] = &data.pinky;
  _ghosts[2] = &data.inky;
  _ghosts[3] = &data.clyde;
}

GhostSoundController::~GhostSoundController() {
  Mix_FreeChunk(_sirenSample);
  Mix_FreeChunk(_frightSample);
  Mix_FreeChunk(_eyesSample);
}

void GhostSoundController::play(Mix_Chunk *chunk, int loops) {
  Mix_HaltChannel(3);
  Mix_PlayChannel(3, chunk, loops);
  _playingNow = chunk;
}

void GhostSoundController::stop() {
  Mix_HaltChannel(3);
  _playingNow = NULL;
}

void GhostSoundController::update() {
  for (size_t i = 0; i < _ghosts.size(); i++) {
    if (_ghosts[i]->state == GhostData::Dead) {
      if (_playingNow != _eyesSample)
        play(_eyesSample, -1);
      return;
    }
  }
  for (size_t i = 0; i < _ghosts.size(); i++) {
    if (_ghosts[i]->state == GhostData::Frightened ||
        _ghosts[i]->state == GhostData::FrightenedAndComingOut) {
      if (_playingNow != _frightSample)
        play(_frightSample, -1);
      return;
    }
  }
  if (_playingNow != _sirenSample) {
    play(_sirenSample, -1);
  }
}
