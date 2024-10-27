#include "GameInterface.hpp"

GameInterface::GameInterface(const int &score, const int &highScore,
                             SDL_Renderer *renderer)
    : _score(score), _highScore(highScore) {
  _renderer = renderer;
  _highScoreTitleTexture.loadFont(FONT_FILE, "HIGH SCORE", 24, &WHITE, _renderer);
  _highScoreTitleTexture.setCoordinates(
      (WINDOW_WIDTH - _highScoreTitleTexture.getWidth()) / 2, 0);

  _highScoreTexture.loadFont(FONT_FILE, "00", 24, &WHITE, _renderer);
  _highScoreTexture.setCoordinates(BLOCK_SIZE * 17 - _highScoreTexture.getWidth(),
                                   _highScoreTexture.getHeight() + 1);

  _playerNumberTexture.loadFont(FONT_FILE, "1UP", 24, &WHITE, _renderer);
  _playerNumberTexture.setCoordinates(BLOCK_SIZE * 3, 0);

  _currentScoreTexture.loadFont(FONT_FILE, "00", 24, &WHITE, _renderer);
  _currentScoreTexture.setCoordinates(BLOCK_SIZE * 7 - _currentScoreTexture.getWidth(),
                                      _playerNumberTexture.getHeight());
}

void GameInterface::draw() const {
  // TODO avoid this
  if (_prevScore != _score) {
    const_cast<GameInterface *>(this)->updateScore(_score);
  }
  if (_prevHighScore != _highScore) {
    const_cast<GameInterface *>(this)->updateHighScore(_highScore);
  }
  _highScoreTitleTexture.draw();
  _highScoreTexture.draw();

  _playerNumberTexture.draw();
  _currentScoreTexture.draw();
  // TODO avoid this
  const_cast<GameInterface *>(this)->_prevScore = _score;
  const_cast<GameInterface *>(this)->_prevHighScore = _highScore;
}

void GameInterface::updateScore(const int &score) {
  if (score == 0)
    _currentScoreTexture.setText("00");
  else
    _currentScoreTexture.setText(std::to_string(score));
  _currentScoreTexture.setCoordinates(BLOCK_SIZE * 7 - _currentScoreTexture.getWidth(),
                                      _playerNumberTexture.getHeight());
}

void GameInterface::updateHighScore(const int &score) {
  if (score == 0)
    _highScoreTexture.setText("");
  else
    _highScoreTexture.setText(std::to_string(score));

  _highScoreTexture.setCoordinates(BLOCK_SIZE * 17 - _highScoreTexture.getWidth(),
                                   _highScoreTexture.getHeight() + 1);
}
