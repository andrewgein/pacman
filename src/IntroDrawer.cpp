#include "IntroDrawer.hpp"

IntroDrawer::IntroDrawer(IntroController &controller, SDL_Renderer *renderer)
    : _controller(controller), _playerDrawer(controller.player, renderer, 0, 0),
      _animationTimer(ANIMATION_TICKS),
      _interface(*controller.score, *controller.highScore, renderer) {
  _animationTimer.start();
  _tableHeader.init(controller.tableHeader, renderer);
  _drawers.push_back(&_tableHeader);

  _ghostDrawers[0] = new BlinkyDrawer(controller.ghostData[0], renderer, ZERO_OFFSET);
  _ghostDrawers[1] = new PinkyDrawer(controller.ghostData[1], renderer, ZERO_OFFSET);
  _ghostDrawers[2] = new InkyDrawer(controller.ghostData[2], renderer, ZERO_OFFSET);
  _ghostDrawers[3] = new ClydeDrawer(controller.ghostData[3], renderer, ZERO_OFFSET);

  const point_t startFrames[] = {{BLINKY_TEXTURE_OFFSET_X, BLINKY_TEXTURE_OFFSET_Y},
                                 {PINKY_TEXTURE_OFFSET_X, PINKY_TEXTURE_OFFSET_Y},
                                 {INKY_TEXTURE_OFFSET_X, INKY_TEXTURE_OFFSET_Y},
                                 {CLYDE_TEXTURE_OFFSET_X, CLYDE_TEXTURE_OFFSET_Y}};

  for (int i = 0; i < 4; i++) {
    _drawers.push_back(_ghostDrawers[i]);

    _ghostIcons[i].init(controller.ghostIcons[i], startFrames[i].x, startFrames[i].y, 40,
                        40, renderer);
    _drawers.push_back(&(_ghostIcons[i]));

    _ghostNames[i].init(controller.ghostNames[i], renderer);
    _drawers.push_back(&(_ghostNames[i]));

    _ghostCharacters[i].init(controller.ghostCharacters[i], renderer);
    _drawers.push_back(&(_ghostCharacters[i]));
  }

  for (int i = 0; i < 4; i++) {
    _points[i].init(controller.points[i], 697 + i * (37 + 5), 366, 19, 37 + (i == 3) * 4,
                    renderer);
    _drawers.push_back(&(_points[i]));
  }

  _food[0].init(controller.lowerBigFood, 1128, 143, 20, 20, renderer);
  _food[1].init(controller.lowerSmallFood, 1128, 138, 5, 5, renderer);
  _food[2].init(controller.bigFood, 1128, 143, 20, 20, renderer);
  for (std::array<ImageDrawer, 3>::iterator it = _food.begin(); it != _food.end(); it++) {
    _drawers.push_back(&(*it));
  }

  _foodPoints[0].init(controller.bigFoodPoints, renderer);
  _foodPoints[1].init(controller.smallFoodPoints, renderer);
  for (std::array<TextDrawer, 4>::iterator it = _foodPoints.begin();
       it != _foodPoints.end(); it++) {
    _drawers.push_back(&(*it));
  }

  _drawers.push_back(&_playerDrawer);
  _drawers.push_back(&_interface);
}

void IntroDrawer::draw() const {
  for (std::list<const Drawable *>::const_iterator it = _drawers.begin();
       it != _drawers.end(); it++) {
    (*it)->draw();
  }
}
