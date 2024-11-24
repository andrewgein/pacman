#include "BlinkyController.hpp"

BlinkyController::BlinkyController(GhostData &data, const Map &map,
                                   const PlayerController &player)
    : GhostController(data, map, player) {
  _cornersPositions[0] = mkcpoint(21, 5);
  _cornersPositions[1] = mkcpoint(21, 1);
  _cornersPositions[2] = mkcpoint(26, 1);
  _cornersPositions[3] = mkcpoint(26, 5);
  _currentCorner = 0;
  reset();
}

void BlinkyController::reset() {
  _data.position = EXIT_POSITION;
  _data.state = _phaseQueue.front().state;
  _data.direction = Left;
  _data.isVisible = true;
  clearPath();
}

void BlinkyController::restart() { reset(); }

void BlinkyController::updateTargetCorner() {
  buildPathTo(getCurrentCorner());
  if (++_currentCorner >= 4) {
    _currentCorner = 0;
  }
}

Timer *BlinkyController::getReleaseTimer() { return NULL; }

point_t BlinkyController::getHomePosition() const { return HOME_POSITION; }

cpoint_t BlinkyController::getCurrentCorner() {
  return _cornersPositions[_currentCorner];
}
