#ifndef __GAME_INTERFACE_H__
#define __GAME_INTERFACE_H__

#include <SDL2/SDL.h>

#include "Drawable.hpp"
#include "Global.hpp"
#include "Texture.hpp"

class GameInterface : public Drawable {
public:
  GameInterface(const int &, const int &, SDL_Renderer *);
  void draw() const;
  void updateScore(const int &);
  void updateHighScore(const int &);

private:
  SDL_Renderer *_renderer;
  const int &_score;
  const int &_highScore;
  int _prevScore;
  int _prevHighScore;
  Texture _highScoreTitleTexture;
  Texture _highScoreTexture;
  Texture _playerNumberTexture;
  Texture _currentScoreTexture;
};

#endif
