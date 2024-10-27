#include "IntroController.hpp"

IntroController::IntroController(GameData *data)
    : _blinkTimer(200), _data(data), _entityUpdateTimer(ENTITY_UPDATE_TICKS),
      _frightenedUpdateTimer(FRIGHTENED_UPDATE_TICKS),
      _ghostWhereEatenTimer(GHOST_WHERE_EATEN_TICKS), score(&data->score),
      highScore(&data->highScore) {

  _foodWhereEaten = false;
  _ghostWhereEaten = false;
  _entityUpdateTimer.start();

  tableHeader = (TextData){"CHARACTER / NICKNAME", &WHITE, 24,
                           (point_t){BLOCK_SIZE * 7, BLOCK_SIZE * 5}, true};

  for (int i = 0; i < 4; i++) {
    ghostData[i] = getNewGhostData(i);
    _ghosts.push_back(&ghostData[i]);
  }

  player = (PlayerData){static_cast<int>(BLOCK_SIZE * 29.5),
                        static_cast<int>(BLOCK_SIZE * 19.8),
                        Left,
                        None,
                        PlayerData::Alive,
                        false,
                        true};

  for (int i = 0; i < 4; i++) {
    points[i] = (ImageData){0, 0, false};
  }

  SDL_Color const *colors[] = {&RED, &PINK, &BLUE, &YELLOW};
  const std::string characters[] = {"-SHADOW", "-SPEEDY", "-BASHFUL", "-POKEY"};
  const std::string names[] = {"\"BLINKY\"", "\"PINKY\"", "\"INKY\"", "\"CLYDE\""};
  for (int i = 0; i < 4; i++) {
    ghostIcons[i] = (ImageData){BLOCK_SIZE * 4, BLOCK_SIZE * (7 + 3 * i) - 2, false};

    ghostCharacters[i] = (TextData){
        characters[i], colors[i], 24, BLOCK_SIZE * 7, BLOCK_SIZE * (7 + 3 * i), false};

    ghostNames[i] = (TextData){
        names[i], colors[i], 24, BLOCK_SIZE * 18, BLOCK_SIZE * (7 + 3 * i), false};

#ifndef NDEBUG
    _animationList.push((AnimationUnit){&(ghostIcons[i].isVisible), new Timer(70)});
    _animationList.push((AnimationUnit){&(ghostCharacters[i].isVisible), new Timer(50)});
    _animationList.push((AnimationUnit){&(ghostNames[i].isVisible), new Timer(50)});
#else
    _animationList.push((AnimationUnit){&(ghostIcons[i].isVisible), new Timer(700)});
    _animationList.push((AnimationUnit){&(ghostCharacters[i].isVisible), new Timer(500)});
    _animationList.push((AnimationUnit){&(ghostNames[i].isVisible), new Timer(500)});
#endif
  }
  lowerSmallFood = (ImageData){static_cast<int>(BLOCK_SIZE * 10.5),
                               static_cast<int>(BLOCK_SIZE * 24.6), false};
  lowerBigFood = (ImageData){static_cast<int>(BLOCK_SIZE * 10.25),
                             static_cast<int>(BLOCK_SIZE * 26.3), false};
  smallFoodPoints = (TextData){"10 pts", &WHITE, 24,
                               (point_t){BLOCK_SIZE * 12, BLOCK_SIZE * 24}, false};
  bigFoodPoints = (TextData){"50 pts", &WHITE, 24,
                             (point_t){BLOCK_SIZE * 12, BLOCK_SIZE * 26}, false};
  bigFood =
      (ImageData){(point_t){BLOCK_SIZE * 4, static_cast<int>(BLOCK_SIZE * 19.4)}, false};

  _animationList.push((AnimationUnit){&lowerSmallFood.isVisible, new Timer(700)});
  _animationList.push((AnimationUnit){&lowerBigFood.isVisible, new Timer(0)});
  _animationList.push((AnimationUnit){&bigFood.isVisible, new Timer(0)});
  _animationList.push((AnimationUnit){&bigFoodPoints.isVisible, new Timer(0)});
  _animationList.push((AnimationUnit){&smallFoodPoints.isVisible, new Timer(0)});
}

void IntroController::update() {
  if (!_animationList.empty()) {
    if (!_animationList.front().timer->isActive()) {
      _animationList.front().timer->start();
    }
    while (!_animationList.empty() && _animationList.front().timer->isTriggered()) {
      (*_animationList.front().isVisible) = true;
      delete _animationList.front().timer;
      _animationList.pop();
      if (!_animationList.empty()) {
        _animationList.front().timer->start();
      } else {
        _blinkTimer.start();
      }
    }
    return;
  }

  if (_ghostWhereEaten) {
    if (_ghostWhereEatenTimer.isTriggered()) {
      _ghostWhereEaten = false;
      points[4 - _ghosts.size()].isVisible = false;
      _ghosts.pop_front();
      player.isVisible = true;
      if (_ghosts.size() == 0) {
        _data->stage = GameData::Readiness;
      }
    }
    return;
  }

  if (_blinkTimer.isTriggered()) {
    lowerBigFood.isVisible = !lowerBigFood.isVisible;
    if (!_foodWhereEaten) {
      bigFood.isVisible = lowerBigFood.isVisible;
    }
    _blinkTimer.start();
  }

  if (Map::isColliding(player.position, bigFood.position)) {
    for (std::list<GhostData *>::iterator it = _ghosts.begin(); it != _ghosts.end();
         it++) {
      (*it)->state = GhostData::Frightened;
      (*it)->direction = Right;
    }
    player.direction = Right;
    _foodWhereEaten = true;
    bigFood.isVisible = false;
    _frightenedUpdateTimer.start();
  }

  if (Map::isColliding(player.position, (*_ghosts.begin())->position)) {
    _ghostWhereEaten = true;
    _ghostWhereEatenTimer.start();
    (*_ghosts.begin())->isVisible = false;
    points[4 - _ghosts.size()].isVisible = true;
    points[4 - _ghosts.size()].position = (*_ghosts.begin())->position;
    points[4 - _ghosts.size()].position.y -= BLOCK_SIZE / 2;
    points[4 - _ghosts.size()].position.x -= BLOCK_SIZE;
    player.isVisible = false;
  }

  if (_entityUpdateTimer.isTriggered()) {
    movePlayer();
    if (!_foodWhereEaten) {
      for (std::list<GhostData *>::iterator it = _ghosts.begin(); it != _ghosts.end();
           it++) {
        moveGhost(*(*it));
      }
    }
  }
  if (_foodWhereEaten && _frightenedUpdateTimer.isTriggered()) {
    for (std::list<GhostData *>::iterator it = _ghosts.begin(); it != _ghosts.end();
         it++) {
      moveGhost(*(*it));
    }
    _frightenedUpdateTimer.start();
  }
}

void IntroController::handleKeyEvent(SDL_KeyboardEvent &event) {
  switch (event.keysym.sym) {
  case SDLK_RETURN:
  case SDLK_ESCAPE:
    _data->stage = GameData::Readiness;
  }
}

void IntroController::movePlayer() {
  if (_foodWhereEaten)
    player.position.x += 2;
  else
    player.position.x -= 2;
}

void IntroController::moveGhost(GhostData &data) {
  if (_foodWhereEaten)
    data.position.x += 2;
  else
    data.position.x -= 2;
}

GhostData IntroController::getNewGhostData(const int &ghostNumber) {
  GhostData result =
      ((GhostData){static_cast<int>(BLOCK_SIZE * (31.8 + 1.8 * ghostNumber)),
                   static_cast<int>(BLOCK_SIZE * 19.8), Left, GhostData::Chase,
                   std::deque<cpoint_t>(), true});
  return result;
}
