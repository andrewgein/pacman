#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#include <deque>

#include "Global.hpp"
#include "Point.h"

struct PlayerData {
  point_t position;
  Direction direction;
  Direction desiredDirection;
  enum State { Alive, Energized, Dying, Dead } state;
  bool isEating;
  bool isVisible;
};

struct GhostData {
  point_t position;
  Direction direction;
  enum State {
    Chase,
    Scatter,
    Frightened,
    Dead,
    ComingOut,
    Waiting,
    FrightenedAndWaiting,
    FrightenedAndComingOut,
  } state;
  std::deque<cpoint_t> path;
  bool isVisible;
};

struct ImageData {
  point_t position;
  bool isVisible;
};

struct GameData {
public:
  PlayerData player;
  GhostData blinky;
  GhostData pinky;
  GhostData inky;
  GhostData clyde;
  enum Stage {
    Intro,
    Readiness,
    MainGame,
    GhostWhereEaten,
    GameOver,
    EndOfGame
  } stage;
  int score;
  int highScore;
  short lives;
  short currentPoints;
  point_t currentPointsPosition;
};

struct TextData {
  std::string text;
  const SDL_Color *color;
  int size;
  point_t position;
  bool isVisible;
};

#endif
