#ifndef __PLAYER_CONTROLLER_H__
#define __PLAYER_CONTROLLER_H__

#include "GameData.hpp"
#include "Global.hpp"
#include "Map.hpp"
#include "Timer.hpp"

class PlayerController {
public:
  PlayerController(PlayerData &, Map &);
  void update();
  void restart();
  void handleKeyEvent(SDL_KeyboardEvent &);
  point_t getPosition() const;
  point_t getNextPosition(Direction);
  Direction getActualDirection() const;
  Direction getDesiredDirection() const;

private:
  void setDirection(Direction);
  short _collisionFlags;
  Timer _updateTimer;
  PlayerData &_data;
  Map &_map;
#ifndef NDEBUG
  const point_t START_POSITION = {static_cast<int>(BLOCK_SIZE * 6.5),
                                  static_cast<int>(BLOCK_SIZE * 14.5)};
#else
  const point_t START_POSITION = {static_cast<int>(BLOCK_SIZE * 14),
                                  static_cast<int>(BLOCK_SIZE * 23.5)};
#endif
};

#endif
