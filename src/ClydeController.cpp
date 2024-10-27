#include "ClydeController.hpp"

ClydeController::ClydeController(GhostData &data, const Map &map,
                                 const PlayerController &player)
    : GhostController(data, map, player), _releaseTimer(RELEASE_TICKS) {
  _cornersPositions[0] = (cpoint_t){9, 23};
  _cornersPositions[1] = (cpoint_t){6, 23};
  _cornersPositions[2] = (cpoint_t){6, 26};
  _cornersPositions[3] = (cpoint_t){1, 26};
  _cornersPositions[4] = (cpoint_t){1, 29};
  _cornersPositions[5] = (cpoint_t){12, 29};
  _cornersPositions[6] = (cpoint_t){12, 26};
  _cornersPositions[7] = (cpoint_t){9, 26};
  _currentCorner = 0;
  restart();
}

void ClydeController::restart() {
  _data.position = HOME_POSITION;
  setState(GhostData::Waiting);
  _isAlongEdgeOfTile = true;
  _data.direction = Up;
  _data.isVisible = true;
  _releaseTimer.deactivate();
  buildPathTo(mkpoint(_data.position.x, HOME_POSITION.y - BLOCK_SIZE));
}

point_t ClydeController::getHomePosition() const { return HOME_POSITION; }

void ClydeController::updateTargetCorner() {
  buildPathTo(getCurrentCorner());
  if (_currentCorner >= 8) {
    _currentCorner = 0;
  }
}

Timer *ClydeController::getReleaseTimer() { return &_releaseTimer; }

cpoint_t ClydeController::getCurrentCorner() { return _cornersPositions[_currentCorner]; }
