#include "PlayerDrawer.hpp"

PlayerDrawer::PlayerDrawer(const PlayerData &data, SDL_Renderer *renderer,
                           const short &offsetX, const short &offsetY)
    : _data(data), _animationTimer(ANIMATION_TICKS) {
  this->_renderer = renderer;
  _currentFrame = 0;
  _deadFrame = 0;
  _isInRewind = true;
  _animationTimer.start();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 2; j++) {
      _playerTextures[i][j] = new Texture();
      _playerTextures[i][j]->setOffset(offsetX, offsetY);
      _playerTextures[i][j]->loadImage(
          SPRITE_SHEET_FILE, _renderer, START_CLIP_X + (FRAME_STEP + WIDTH) * j,
          START_CLIP_Y + (FRAME_STEP + HEIGHT) * i, WIDTH, HEIGHT);
    }
    if (i) {
      _playerTextures[i][2] = _playerTextures[0][2];
    } else {
      _playerTextures[i][2] = new Texture();
      _playerTextures[i][2]->setOffset(offsetX, offsetY);
      _playerTextures[i][2]->loadImage(SPRITE_SHEET_FILE, _renderer,
                                       START_CLIP_X + (FRAME_STEP + WIDTH) * 2,
                                       START_CLIP_Y, WIDTH, HEIGHT);
    }
  }

  for (int i = 0; i < 4; i++) {
    if (i == 3) {
      _playerPointerTextures[i] = new Texture();
      _playerPointerTextures[i]->loadImage(
          SPRITE_SHEET_FILE, _renderer, START_POINTER_CLIP_X,
          START_POINTER_CLIP_Y + i * POINTER_CLIP_STEP - 6, POINTER_HEIGHT,
          POINTER_WIDTH);
      continue;
    }
    if (i > 1) {
      _playerPointerTextures[i] = new Texture();
      _playerPointerTextures[i]->loadImage(
          SPRITE_SHEET_FILE, _renderer, START_POINTER_CLIP_X,
          START_POINTER_CLIP_Y + i * POINTER_CLIP_STEP, POINTER_HEIGHT, POINTER_WIDTH);
    } else {
      _playerPointerTextures[i] = new Texture();
      _playerPointerTextures[i]->loadImage(
          SPRITE_SHEET_FILE, _renderer, START_POINTER_CLIP_X,
          START_POINTER_CLIP_Y + i * POINTER_CLIP_STEP, POINTER_WIDTH, POINTER_HEIGHT);
    }
  }

  for (int i = 0; i < 4; i++) {
    _playerPointerTextures[i]->setOffset(offsetX, offsetY);
  }

  for (int i = 0; i < DEAD_FRAME_COUNT; i++) {
    _deadAnimationTextures[i] = new Texture();
    _deadAnimationTextures[i]->loadImage(SPRITE_SHEET_FILE, _renderer,
                                         START_DEAD_CLIP_X +
                                             i * (DEAD_FRAME_STEP + WIDTH + 5),
                                         START_DEAD_CLIP_Y, WIDTH, HEIGHT);
    _deadAnimationTextures[i]->setOffset(offsetX, offsetY);
  }
}

void PlayerDrawer::updateFrame() {
  if (_data.state == PlayerData::Dying) {
    _deadFrame++;
    if (isDeathAnimationEnd()) {
      _deadFrame = 0;
      // TODO avoid this
      const_cast<PlayerData &>(const_cast<PlayerDrawer *>(this)->_data).state =
          PlayerData::Dead;
    }
    return;
  }
  if (_isInRewind) {
    _currentFrame--;
  } else {
    _currentFrame++;
  }
  if (_currentFrame >= FRAME_COUNT || _currentFrame < 0) {
    _currentFrame = 1;
    _isInRewind = !_isInRewind;
  }
}

void PlayerDrawer::draw() const {
  if (!_data.isVisible) {
    return;
  }
  point_t position = _data.position;
  position.x -= WIDTH / 2;
  position.y -= HEIGHT / 2;
  if (_data.state == PlayerData::Dying) {
    if (_deadFrame == DEAD_FRAME_COUNT) {
      return;
    }
    _deadAnimationTextures[_deadFrame]->draw(position);
  } else {
    _playerTextures[_data.direction][_currentFrame]->draw(position);
    drawPointer();
  }
  /* TODO avoid this */
  if (const_cast<PlayerDrawer *>(this)->_animationTimer.isTriggered()) {
    const_cast<PlayerDrawer *>(this)->updateFrame();
    const_cast<PlayerDrawer *>(this)->_animationTimer.start();
  }
}

void PlayerDrawer::drawPointer() const {
  Direction state = _data.desiredDirection;
  short x = _data.position.x - WIDTH / 2;
  short y = _data.position.y - HEIGHT / 2;

  switch (state) {
  case Right:
    _playerPointerTextures[state]->draw(x + WIDTH + POINTER_DISTANCE,
                                        y + (HEIGHT - POINTER_HEIGHT) / 2);
    break;
  case Left:
    _playerPointerTextures[state]->draw(x - POINTER_DISTANCE - POINTER_WIDTH,
                                        y + (HEIGHT - POINTER_HEIGHT) / 2);
    break;
  case Up:
    _playerPointerTextures[state]->draw(x + (WIDTH - POINTER_HEIGHT) / 2,
                                        y - POINTER_DISTANCE - POINTER_WIDTH);
    break;
  case Down:
    _playerPointerTextures[state]->draw(x + (WIDTH - POINTER_HEIGHT) / 2,
                                        y + HEIGHT + POINTER_DISTANCE);
  default:
    break;
  }
}

bool PlayerDrawer::isDeathAnimationEnd() {
  if (_deadFrame == DEAD_FRAME_COUNT) {
    _deadFrame = 0;
    return true;
  }
  return false;
}
