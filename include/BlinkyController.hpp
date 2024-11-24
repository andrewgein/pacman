#ifndef __BLINKY_CONTROLLER_H__
#define __BLINKY_CONTROLLER_H__

#include <array>
#include <deque>

#include "GhostController.hpp"
#include "Map.hpp"
#include "PlayerController.hpp"
#include "Point.h"

class BlinkyController : public GhostController {
public:
  BlinkyController(GhostData &data, const Map &, const PlayerController &);
  void restart();

private:
  void reset();
  cpoint_t _cornersPositions[4];
  cpoint_t getCurrentCorner();
  short _currentCorner;
  void updateTargetCorner();
  Timer *getReleaseTimer();
  point_t getHomePosition() const;
  const point_t HOME_POSITION = {BLOCK_SIZE * 14, BLOCK_SIZE * 14};
};

#endif
