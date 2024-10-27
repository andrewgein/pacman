#include "GameController.hpp"

GameController::GameController(GameData &data, Map &map)
    : _map(map), _data(data), _playerController(_data.player, _map),
      _energizedTimer(ENERGIZED_TICKS), _readinessTimer(READINESS_TICKS),
      _ghostWhereEatenTimer(GHOST_WHERE_EATEN_TICKS), _soundController(data) {
  _ghosts[0] = new BlinkyController(_data.blinky, _map, _playerController);
  _ghosts[1] = new PinkyController(_data.pinky, _map, _playerController);
  _ghosts[2] = new InkyController(_data.inky, _map, _playerController);
  _ghosts[3] = new ClydeController(_data.clyde, _map, _playerController);
}

void GameController::update() {
  //_soundController.update();
  if (_data.stage == GameData::Readiness) {
    if (!_readinessTimer.isActive()) {
      _readinessTimer.start();
    }
    if (_readinessTimer.isTriggered())
      _data.stage = GameData::MainGame;
    return;
  }
  if (_data.player.state == PlayerData::Dying) {
    return;
  }
  if (_data.player.state == PlayerData::Dead) {
    _data.lives--;
    if (_data.lives == 0) {
      _data.stage = GameData::GameOver;
    } else {
      restart();
      _readinessTimer.start();
      _data.stage = GameData::Readiness;
    }
    return;
  }
  if (_data.stage == GameData::GhostWhereEaten) {
    if (_ghostWhereEatenTimer.isTriggered()) {
      _data.stage = GameData::MainGame;
      for (size_t i = 0; i < _ghosts.size(); i++)
        _ghosts[i]->setVisibility(true);
      _data.currentPoints++;
    } else {
      return;
    }
  }

  _playerController.update();
  for (size_t i = 0; i < _ghosts.size(); i++)
    _ghosts[i]->update();

  // TODO
  short foodPoints = _map.foodCollision(_playerController.getPosition());
  updateScore(foodPoints);
  if (foodPoints > 0) {
    _data.player.isEating = true;
  } else {
    _data.player.isEating = false;
  }
  if (foodPoints >= 50) {
    _data.player.state = PlayerData::Energized;
    _data.currentPoints = 0;
    for (size_t i = 0; i < _ghosts.size(); i++) {
      if (_ghosts[i]->getState() != GhostData::Dead)
        _ghosts[i]->setState(GhostData::Frightened);
    }
    _energizedTimer.start();
  }

  for (size_t i = 0; i < _ghosts.size(); i++) {
    if (Map::isColliding(_data.player.position, _ghosts[i]->getPosition())) {
      if (_ghosts[i]->isFrightened()) {
        _ghosts[i]->setState(GhostData::Dead);
        _data.stage = GameData::GhostWhereEaten;
        _ghostWhereEatenTimer.start();
        _data.currentPointsPosition = _ghosts[i]->getPosition();
        _ghosts[i]->setVisibility(false);
        updateScore(200);
      } else if (_ghosts[i]->getState() != GhostData::Dead) {
        _data.player.state = PlayerData::Dying;
        for (size_t j = 0; j < _ghosts.size(); j++) {
          _ghosts[j]->setVisibility(false);
        }
        return;
      }
    }
  }

  if (_energizedTimer.isTriggered()) {
    _energizedTimer.deactivate();
    _data.player.state = PlayerData::Alive;
    for (size_t i = 0; i < _ghosts.size(); i++) {
      if (_ghosts[i]->isFrightened()) {
        _ghosts[i]->continueState();
      }
    }
  }
}

void GameController::restart() {
  for (size_t i = 0; i < _ghosts.size(); i++)
    _ghosts[i]->restart();
  _playerController.restart();
}

void GameController::handleKeyEvent(SDL_KeyboardEvent &event) {
#ifndef NDEBUG
  if (event.keysym.sym == SDLK_d) {
    for (size_t i = 0; i < _ghosts.size(); i++) {
      if (_ghosts[i]->getState() != GhostData::ComingOut &&
          _ghosts[i]->getState() != GhostData::Waiting) {
        _ghosts[i]->setState(GhostData::Frightened);
        _ghosts[i]->setState(GhostData::Dead);
      }
    }
  }
#endif
  _playerController.handleKeyEvent(event);
}

void GameController::updateScore(const int &toAdd) {
  _data.score += toAdd;
  _data.highScore = std::max(_data.score, _data.highScore);
}

GameController::~GameController() {
  for (size_t i = 0; i < _ghosts.size(); i++) {
    delete _ghosts[i];
  }
}
