#ifndef __INTRO_CONTROLLER__
#define __INTRO_CONTROLLER__

#include <algorithm>
#include <list>
#include <queue>

#include "GameData.hpp"
#include "GameInterface.hpp"
#include "Map.hpp"
#include "Texture.hpp"
#include "Timer.hpp"

class IntroController {
public:
  IntroController(GameData *);
  void update();
  void handleKeyEvent(SDL_KeyboardEvent &);
  TextData tableHeader;
  TextData ghostCharacters[4];
  TextData smallFoodPoints, bigFoodPoints;
  ImageData lowerSmallFood, lowerBigFood, bigFood;
  TextData ghostNames[4];
  ImageData ghostIcons[4];
  ImageData points[4];
  PlayerData player;
  GhostData ghostData[4];
  GameData *_data;
  int *score;
  int *highScore;

private:
  typedef struct _AnimationUnit {
    bool *isVisible;
    Timer *timer;
  } AnimationUnit;
  struct {
    Texture texture;
    point_t position;
  } _food;
  void moveGhost(GhostData &);
  void movePlayer();
  bool isCollidingWithPlayer(const GhostData &);
  GhostData getNewGhostData(const int &);
  SDL_Renderer *_renderer;
  Texture _tableHeader;
  std::list<GhostData *> _ghosts;
  std::queue<AnimationUnit> _animationList;
  Timer _blinkTimer;
  bool _ghostWhereEaten;
  bool _foodWhereEaten;
  bool _isBlinkingTextureVisible;
  Timer _entityUpdateTimer;
  Timer _frightenedUpdateTimer;
  Timer _ghostWhereEatenTimer;
  static const int ENTITY_UPDATE_TICKS = 0;
  static const int FRIGHTENED_UPDATE_TICKS = 17;
  static const int GHOST_WHERE_EATEN_TICKS = 1300;
};

#endif
