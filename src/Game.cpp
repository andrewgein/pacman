#include "Game.hpp"

Game::Game(SDL_Renderer *renderer)
    : _map(renderer), _introController(&_data), _introDrawer(_introController, renderer),
      _gameController(_data, _map), _gameDrawer(_data, _map, renderer) {

  _data.stage = GameData::Intro;
  _data.lives = 3;
  setHighScore();
}

void Game::draw() {
  switch (_data.stage) {
  case GameData::Intro:
    _introDrawer.draw();
    break;
  case GameData::Readiness:
  case GameData::MainGame:
  case GameData::GhostWhereEaten:
  case GameData::GameOver:
    _gameDrawer.draw();
    break;
  default:
    break;
  }
}

void Game::update() {
  switch (_data.stage) {
  case GameData::Intro:
    _introController.update();
    break;
  case GameData::Readiness:
  case GameData::GhostWhereEaten:
  case GameData::MainGame:
    _gameController.update();
    break;
  case GameData::GameOver:
    break;
  case GameData::EndOfGame:
    break;
  }
}

void Game::keyUp(SDL_KeyboardEvent &event) {}
void Game::keyDown(SDL_KeyboardEvent &event) {
  switch (_data.stage) {
  case GameData::Intro:
    _introController.handleKeyEvent(event);
    break;
  case GameData::MainGame:
    _gameController.handleKeyEvent(event);
    break;
  default:
    break;
  }
}

void Game::setHighScore() {
  _data.score = 0;
  _data.highScore = 0;
  std::ifstream file("game.dat");
  if (file.is_open() && !file.eof()) {
    file >> _data.highScore;
  }
  file.close();
}

void Game::writeHighScore() {
  std::ofstream file("game.dat");
  file << _data.highScore << std::endl;
  file.close();
}
