#ifndef __PINKY_CONTROLLER_H__
#define __PINKY_CONTROLLER_H__

#include "GhostController.hpp"

class PinkyController : public GhostController {
public:
  PinkyController(GhostData &, const Map &, const PlayerController &);
  void restart();

private:
  void reset();
  cpoint_t _cornersPositions[4];
  short _currentCorner;
  void updateTargetCorner();
  Timer *getReleaseTimer();
  point_t getHomePosition() const;
  cpoint_t getCurrentCorner();
  const point_t HOME_POSITION = {BLOCK_SIZE * 14, static_cast<int>(BLOCK_SIZE * 14.5)};
};

#endif
