#include "GhostDrawer.hpp"

GhostDrawer::GhostDrawer(const GhostData &ghostData, SDL_Renderer *renderer,
                         const point_t &textureOffset, const point_t &mapOffset)
    : _data(ghostData), _animationTimer(ANIMATION_TICKS) {

  this->_renderer = renderer;
  _animationTimer.start();

  for (int i = 0; i < 8; i += 2) {
    for (int j = 0; j < 2; j++) {
      _ghostTextures[i / 2][j] = new Texture();
      _ghostTextures[i / 2][j]->loadImage(
          SPRITE_SHEET_FILE, _renderer, textureOffset.x + (FRAME_STEP + WIDTH) * (i + j),
          textureOffset.y, WIDTH, HEIGHT);
      _ghostTextures[i / 2][j]->setOffset(mapOffset.x, mapOffset.y);
    }
  }
  for (int i = 0; i < 2; i++) {
    _frightenedTextures[i] = new Texture();
    _frightenedTextures[i]->loadImage(SPRITE_SHEET_FILE, _renderer,
                                      START_FRIGHTENED_FRAME_X + (WIDTH + FRAME_STEP) * i,
                                      START_FRIGHTENED_FRAME_Y, WIDTH, HEIGHT);
    _frightenedTextures[i]->setOffset(mapOffset.x, mapOffset.y);
  }
  for (int i = 0; i < 4; i++) {
    _deadTextures[i] = new Texture();
    _deadTextures[i]->loadImage(SPRITE_SHEET_FILE, renderer,
                                START_DEAD_CLIP_X + (WIDTH + FRAME_STEP) * i,
                                START_DEAD_CLIP_Y, WIDTH, HEIGHT);
    _deadTextures[i]->setOffset(mapOffset.x, mapOffset.y);
  }
  _currentFrame = 0;

#ifndef NDEBUG
  _pointTexture = new Texture();
  _pointTexture->loadImage(SPRITE_SHEET_FILE, renderer, textureOffset.x + WIDTH / 2,
                           textureOffset.y + 5, 6, 6);
  _pointTexture->setOffset(mapOffset.x + BLOCK_SIZE / 2 - 2,
                           mapOffset.y + BLOCK_SIZE / 2 - 2);
  _positionTexture = new Texture();
  _positionTexture->loadFont("resources/fonts/Emulogic.ttf", "5555", 12, &WHITE,
                             _renderer);
  _positionTexture->setCoordinates(BLOCK_SIZE * 20, 0);
  _cellPositionTexture = new Texture();
  _cellPositionTexture->loadFont("resources/fonts/Emulogic.ttf", "5555", 12, &WHITE,
                                 _renderer);
  _cellPositionTexture->setCoordinates(BLOCK_SIZE * 20, BLOCK_SIZE);
#endif
}

void GhostDrawer::draw() const {
  if (_data.direction == None || !_data.isVisible) {
    return;
  }
  point_t position = _data.position;
  position.x -= WIDTH / 2;
  position.y -= HEIGHT / 2;
  switch (_data.state) {
  case GhostData::Frightened:
  case GhostData::FrightenedAndWaiting:
  case GhostData::FrightenedAndComingOut:
    _frightenedTextures[_currentFrame]->draw(position);
    break;
  case GhostData::Dead:
    _deadTextures[_data.direction]->draw(position);
    break;
  default:
    _ghostTextures[_data.direction][_currentFrame]->draw(position);
  }
  /* TODO avoid this */
  if (const_cast<GhostDrawer *>(this)->_animationTimer.isTriggered()) {
    const_cast<GhostDrawer *>(this)->updateFrame();
    const_cast<GhostDrawer *>(this)->_animationTimer.start();
  }

#ifndef NDEBUG
  drawPath();
  _positionTexture->setText(std::to_string(_data.position.x) + " " +
                            std::to_string(_data.position.y));
  _positionTexture->draw();
  cpoint_t cellPoint = _data.position;
  _cellPositionTexture->setText(std::to_string(cellPoint.x) + " " +
                                std::to_string(cellPoint.y));
  _cellPositionTexture->draw();
#endif
}

void GhostDrawer::updateFrame() {
  _currentFrame++;
  if (_currentFrame >= FRAME_COUNT) {
    _currentFrame = 0;
  }
}

#ifndef NDEBUG
void GhostDrawer::drawPath() const {
  for (std::deque<cpoint_t>::const_iterator it = _data.path.begin();
       it != _data.path.end(); it++) {
    _pointTexture->draw(it->x * BLOCK_SIZE, it->y * BLOCK_SIZE);
  }
}
#endif
