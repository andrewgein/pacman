#ifndef __INTRO_DRAWER_H__
#define __INTRO_DRAWER_H__

#include <array>
#include <list>
#include <memory>
#include <vector>

#include "BlinkyDrawer.hpp"
#include "ClydeDrawer.hpp"
#include "Drawable.hpp"
#include "GhostDrawer.hpp"
#include "Global.hpp"
#include "InkyDrawer.hpp"
#include "IntroController.hpp"
#include "PinkyDrawer.hpp"
#include "PlayerDrawer.hpp"

class IntroDrawer : public Drawable {
public:
  IntroDrawer(IntroController &, SDL_Renderer *renderer);
  void draw() const;

private:
  class TextDrawer : public Drawable {
  public:
    TextDrawer() {}
    void init(const TextData &data, SDL_Renderer *renderer) {
      _data = &data;
      texture.loadFont(FONT_FILE, data.text, data.size, data.color, renderer);
    }
    void draw() const {
      if (_data->isVisible)
        texture.draw(_data->position.x, _data->position.y);
    }

  private:
    const TextData *_data = NULL;
    Texture texture;
  };

  class ImageDrawer : public Drawable {
  public:
    ImageDrawer() {}
    void init(const ImageData &data, const short &startX, const short &startY,
              const short &height, const short &width, SDL_Renderer *renderer) {
      _data = &data;
      texture.loadImage(SPRITE_SHEET_FILE, renderer, startX, startY, width, height);
    }
    void draw() const {
      if (_data->isVisible)
        texture.draw(_data->position.x, _data->position.y);
    }

  private:
    Texture texture;
    const ImageData *_data = NULL;
  };
  static const Drawable *getPointer(const ImageDrawer &i) { return &i; }
  std::list<const Drawable *> _drawers;

  Timer _animationTimer;

  TextDrawer _tableHeader;
  GameInterface _interface;
  std::array<ImageDrawer, 4> _ghostIcons;
  std::array<TextDrawer, 4> _ghostCharacters;
  std::array<TextDrawer, 4> _ghostNames;
  IntroController &_controller;
  PlayerDrawer _playerDrawer;
  std::array<GhostDrawer *, 4> _ghostDrawers;
  std::array<ImageDrawer, 4> _points;
  std::array<ImageDrawer, 3> _food;
  std::array<TextDrawer, 2> _foodPoints;
  const point_t ZERO_OFFSET = {0, 0};
};

#endif
