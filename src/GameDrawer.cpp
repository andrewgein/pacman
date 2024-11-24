#include "GameDrawer.hpp"

GameDrawer::GameDrawer(const GameData &data, const Map &map, SDL_Renderer *renderer)
    : _data(&data), _gameInterface(_data->score, _data->highScore, renderer), _map(map),
      _renderer(renderer),
      _playerGraphics(_data->player, renderer, MAP_OFFSET_X, MAP_OFFSET_Y) {

  _gameOverTitle.loadFont(FONT_FILE, "GAME  OVER", 24, &RED, renderer);
  _gameOverTitle.setCoordinates((WINDOW_WIDTH - _gameOverTitle.getWidth()) / 2,
                                BLOCK_SIZE * 19.9);
  _liveTexture.loadImage(SPRITE_SHEET_FILE, renderer, 1056, 50, 27, 30);
  _readyTitle.loadFont(FONT_FILE, "Ready!", 24, &YELLOW, renderer);
  _readyTitle.setCoordinates((WINDOW_WIDTH - _readyTitle.getWidth()) / 2,
                             BLOCK_SIZE * 19.9);
  _ghosts[0] = new BlinkyDrawer(_data->blinky, renderer, MAP_OFFSET);
  _ghosts[1] = new PinkyDrawer(_data->pinky, renderer, MAP_OFFSET);
  _ghosts[2] = new InkyDrawer(_data->inky, renderer, MAP_OFFSET);
  _ghosts[3] = new ClydeDrawer(_data->clyde, renderer, MAP_OFFSET);

  for (int i = 0; i < 4; i++) {
    _points[i].loadImage(SPRITE_SHEET_FILE, renderer, 697 + i * (37 + 5), 366,
                         37 + (i == 3) * 4, 19);
    _points[i].setOffset(MAP_OFFSET_X - (_points[i].getWidth() / 2),
                         MAP_OFFSET_Y - (_points[i].getHeight() / 2));
  }
}

GameDrawer::~GameDrawer() {
  for (size_t i = 0; i < _ghosts.size(); i++) {
    delete _ghosts[i];
  }
}

void GameDrawer::draw() const {
  assert(_data->stage != GameData::Intro);
  switch (_data->stage) {
  case GameData::GameOver:
    _gameInterface.draw();
    _map.draw();
    _gameOverTitle.draw();
    break;
  case GameData::Readiness:
    _readyTitle.draw();
  case GameData::MainGame:
    _gameInterface.draw();
    _map.draw();
    for (size_t i = 0; i < _ghosts.size(); i++)
      _ghosts[i]->draw();
    _playerGraphics.draw();
    drawLives();
    break;
  case GameData::GhostWhereEaten:
    _gameInterface.draw();
    _map.draw();
    for (size_t i = 0; i < _ghosts.size(); i++) {
      _ghosts[i]->draw();
    }
    _points[_data->currentPoints].draw(_data->currentPointsPosition);
    drawLives();
    break;
  default:
    break;
  }
  /*#ifndef NDEBUG
    drawTiles();
  #endif*/
}

void GameDrawer::drawLives() const {
  for (int i = 0; i < _data->lives; i++) {
    _liveTexture.draw(BLOCK_SIZE * 2.5 + i * (BLOCK_SIZE * 2),
                      WINDOW_HEIGHT - BLOCK_SIZE * 1.6);
  }
}

#ifndef NDEBUG
void GameDrawer::drawTiles() const {
  SDL_SetRenderDrawColor(_renderer, 0, 200, 0, 255);
  for (int i = 1; i < GAME_WIDTH; i++) {
    SDL_RenderDrawLine(_renderer, BLOCK_SIZE * i, 0, BLOCK_SIZE * i,
                       GAME_HEIGHT * BLOCK_SIZE);
  }
  for (int i = 1; i < GAME_HEIGHT; i++) {
    SDL_RenderDrawLine(_renderer, 0, BLOCK_SIZE * i, GAME_WIDTH * BLOCK_SIZE,
                       BLOCK_SIZE * i);
  }
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
}
#endif
