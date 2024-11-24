#include "InkyController.hpp"

InkyController::InkyController(GhostData &data, const Map &map,
                               const PlayerController &player)
    : GhostController(data, map, player), _releaseTimer(RELEASE_TICKS) {
  _cornersPositions[0] = (cpoint_t){21, 23};
  _cornersPositions[1] = (cpoint_t){21, 26};
  _cornersPositions[2] = (cpoint_t){26, 26};
  _cornersPositions[3] = (cpoint_t){26, 29};
  _cornersPositions[4] = (cpoint_t){15, 29};
  _cornersPositions[5] = (cpoint_t){15, 26};
  _cornersPositions[6] = (cpoint_t){18, 26};
  _cornersPositions[7] = (cpoint_t){18, 23};
  _currentCorner = 0;
  reset();
}

void InkyController::reset() {
  _data.position = HOME_POSITION;
  setState(GhostData::Waiting);
  _isAlongEdgeOfTile = true;
  _data.direction = Up;
  _data.isVisible = true;
  _releaseTimer.deactivate();
  buildPathTo(mkpoint(_data.position.x, HOME_POSITION.y - BLOCK_SIZE));
}

void InkyController::restart() { reset(); }

point_t InkyController::getHomePosition() const { return HOME_POSITION; };

void InkyController::updateTargetCorner() {
  buildPathTo(getCurrentCorner());
  if (++_currentCorner >= 8) {
    _currentCorner = 0;
  }
}

Timer *InkyController::getReleaseTimer() { return &_releaseTimer; }

cpoint_t InkyController::getCurrentCorner() { return _cornersPositions[_currentCorner]; }
