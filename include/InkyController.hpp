#ifndef __INKY_CONTROLLER_H__
#define __INKY_CONTROLLER_H__

#include "GhostController.hpp"

class InkyController : public GhostController {
public:
  InkyController(GhostData &, const Map &, const PlayerController &);
  void restart();

private:
  cpoint_t _cornersPositions[8];
  short _currentCorner;
  void updateTargetCorner();
  Timer *getReleaseTimer();
  point_t getHomePosition() const;
  cpoint_t getCurrentCorner();
  const point_t HOME_POSITION = {BLOCK_SIZE * 12, static_cast<int>(BLOCK_SIZE * 14.5)};

  Timer _releaseTimer;
  const static short RELEASE_TICKS = 9000;
};

#endif
