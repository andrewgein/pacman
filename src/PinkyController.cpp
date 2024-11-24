#include "PinkyController.hpp"

PinkyController::PinkyController(GhostData &data, const Map &map,
                                 const PlayerController &player)
    : GhostController(data, map, player) {
  _cornersPositions[0] = (cpoint_t){6, 5};
  _cornersPositions[1] = (cpoint_t){6, 1};
  _cornersPositions[2] = (cpoint_t){1, 1};
  _cornersPositions[3] = (cpoint_t){1, 5};
  _currentCorner = 0;
  reset();
}

void PinkyController::reset() {
  _data.position = HOME_POSITION;
  setState(GhostData::ComingOut);
  _isAlongEdgeOfTile = true;
  _data.direction = Down;
  _data.isVisible = true;
}

void PinkyController::restart() { reset(); }

void PinkyController::updateTargetCorner() {
  buildPathTo(getCurrentCorner());
  if (++_currentCorner >= 4) {
    _currentCorner = 0;
  }
}

Timer *PinkyController::getReleaseTimer() { return NULL; }

point_t PinkyController::getHomePosition() const { return HOME_POSITION; }

cpoint_t PinkyController::getCurrentCorner() { return _cornersPositions[_currentCorner]; }
