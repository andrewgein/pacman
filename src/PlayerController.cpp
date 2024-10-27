#include "PlayerController.hpp"

PlayerController::PlayerController(PlayerData &data, Map &map)
    : _data(data), _map(map), _updateTimer(PLAYER_UPDATE_TICKS) {
  _collisionFlags = CanUseTeleport;
  _updateTimer.start();
  restart();
}

void PlayerController::restart() {
  _data.position = START_POSITION;
  _data.direction = Up;
  _data.state = PlayerData::Alive;
  _data.desiredDirection = None;
  _data.isVisible = true;
}

void PlayerController::update() {
  if (!_updateTimer.isTriggered()) {
    return;
  }
  _updateTimer.start();
  if (_data.desiredDirection != None) {
    point_t nextPosition = getNextPosition(_data.desiredDirection);
    if (!_map.wallCollision(nextPosition, _collisionFlags)) {
      _data.direction = _data.desiredDirection;
    }
  }
  point_t nextPosition = getNextPosition(_data.direction);
  if (!_map.wallCollision(nextPosition, _collisionFlags)) {
    _data.position = nextPosition;
  }
  if (((cpoint_t)_data.position).y == MAP_TELEPORT_Y) {
    if (_data.position.x + BLOCK_SIZE / 2 <= 0) {
      _data.position.x += MAP_WIDTH + BLOCK_SIZE / 2;
    } else if (_data.position.x >= MAP_WIDTH + BLOCK_SIZE / 2) {
      _data.position.x -= (MAP_WIDTH + BLOCK_SIZE / 2);
    }
  }
}

void PlayerController::handleKeyEvent(SDL_KeyboardEvent &event) {
  switch (event.keysym.sym) {
  case SDLK_k:
  case SDLK_UP:
    _data.desiredDirection = Up;
    break;
  case SDLK_j:
  case SDLK_DOWN:
    _data.desiredDirection = Down;
    break;
  case SDLK_h:
  case SDLK_LEFT:
    _data.desiredDirection = Left;
    break;
  case SDLK_l:
  case SDLK_RIGHT:
    _data.desiredDirection = Right;
    break;
  }
}

void PlayerController::setDirection(Direction direction) { _data.direction = direction; }

point_t PlayerController::getPosition() const { return _data.position; }

Direction PlayerController::getActualDirection() const { return _data.direction; }

Direction PlayerController::getDesiredDirection() const { return _data.desiredDirection; }

point_t PlayerController::getNextPosition(Direction direction) {
  point_t nextPosition = _data.position;
  switch (direction) {
  case Up:
    nextPosition.y -= 2;
    break;
  case Down:
    nextPosition.y += 2;
    break;
  case Left:
    nextPosition.x -= 2;
    break;
  case Right:
    nextPosition.x += 2;
    break;
  default:
    break;
  }
  return nextPosition;
}
