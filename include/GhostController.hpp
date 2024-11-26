#ifndef __GHOST_CONTROLLER_H__
#define __GHOST_CONTROLLER_H__

#include <algorithm>
#include <array>
#include <cassert>
#include <deque>
#include <map>
#include <numeric>

#include "GameData.hpp"
#include "Global.hpp"
#include "Map.hpp"
#include "PlayerController.hpp"
#include "Point.h"
#include "Timer.hpp"

class GhostController {
public:
  GhostController(GhostData &, const Map &, const PlayerController &);
  virtual ~GhostController();
  void update();
  void adjustPath(const PlayerController &);
  virtual void restart() = 0;
  void goHome();
  void goToExit();
  void setState(GhostData::State state);
  void continueState();
  GhostData::State getState();
  void setDirection(Direction);
  Direction getDirection() const;
  point_t getPosition() const;
  void playerPositionChanged();
  void setVisibility(bool);
  bool isVisible();
  bool isFrightened();

protected:
  typedef struct {
    GhostData::State state;
    Timer timer;
  } phase_data;
  std::deque<phase_data> _phaseQueue;
  Timer _updateTimer;
  virtual void updateTargetCorner() = 0;
  void buildPathTo(cpoint_t);
  virtual cpoint_t getCurrentCorner() = 0;
  virtual point_t getHomePosition() const = 0;
  void updateDirection();
  void reverseDirection();
  point_t getNextPosition() const;
  point_t getNextPosition(Direction) const;
  cpoint_t *getTargetPoint() const;
  void updateTargetPoint();
  void clearPath();
  void randomizePath();
  void move();
  void onAfterMove();
  void optimizePath();

  // TODO get rid of setPath, use only buildPathTo instead
  void setPath(const std::vector<cpoint_t> &);

  bool isOnPorch();
  bool canChangeDirection();
  virtual Timer *getReleaseTimer() = 0;
  bool _isAlongEdgeOfTile;
  short _collisionFlags;
  const Map &_map;
  GhostData &_data;
  const PlayerController &_player;
  const point_t EXIT_POSITION = {static_cast<int>(BLOCK_SIZE * 14.5),
                                 static_cast<int>(BLOCK_SIZE * 11.5)};
  const cpoint_t PORCH_POSITION_L = {13, 11};
  const cpoint_t PORCH_POSITION_R = {14, 11};
  const cpoint_t HOME_CENTER_POSITION = {14, 14};
};

#endif
