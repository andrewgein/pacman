#include "Texture.hpp"

Texture::Texture() {
  type = NONE;
  clip = nullptr;
  texture = nullptr;
  font = nullptr;
  renderer = nullptr;
  rectangle = new SDL_Rect();
  rectangle->w = -1;
  rectangle->h = -1;
  rectangle->x = 0;
  rectangle->y = 0;
  _offsetX = 0;
  _offsetY = 0;
}

int Texture::loadFont(const std::string &file, int fontSize, const SDL_Color *color,
                      SDL_Renderer *_renderer) {
  return loadFont(file, "", fontSize, color, _renderer);
}

int Texture::loadFont(const std::string &file, const std::string &_text, int fontSize,
                      const SDL_Color *color, SDL_Renderer *_renderer) {

  renderer = _renderer;
  type = FONT;
  font = FC_CreateFont();
  text = _text;
  if (!FC_LoadFont(font, renderer, file.c_str(), fontSize,
                   FC_MakeColor(color->r, color->g, color->b, color->a),
                   TTF_STYLE_NORMAL)) {

    throw TextureException();
    return 1;
  }
  return 0;
}

int Texture::loadImage(const std::string &file, SDL_Renderer *_renderer,
                       SDL_Rect *_clip = nullptr) {
  type = IMAGE;
  renderer = _renderer;
  clip = _clip;
  texture = IMG_LoadTexture(_renderer, file.c_str());
  if (texture == nullptr) {
    throw TextureException();
  }
  if (_clip != nullptr) {
    rectangle->w = _clip->w;
    rectangle->h = _clip->h;
  } else {
    SDL_QueryTexture(texture, NULL, NULL, &rectangle->w, &rectangle->h);
  }

  return 0;
}

int Texture::loadImage(const std::string &file, SDL_Renderer *_renderer, const int &clipX,
                       const int &clipY, const int &clipW, const int &clipH) {

  type = IMAGE;
  clip = new SDL_Rect();
  clip->x = clipX;
  clip->y = clipY;
  clip->w = clipW;
  clip->h = clipH;

  return loadImage(file, _renderer, clip);
}

void Texture::draw(point_t position) const { draw(position.x, position.y); }

void Texture::draw(int x, int y) const {
  rectangle->x = x + _offsetX;
  rectangle->y = y + _offsetY;
  draw();
}

void Texture::draw() const {
  switch (type) {
  case IMAGE:
    SDL_RenderCopy(renderer, texture, clip, rectangle);
    break;
  case FONT:
    FC_Draw(font, renderer, rectangle->x, rectangle->y, text.c_str());
    break;
  case NONE:
    break;
  }
}

void Texture::setCoordinates(int x, int y) {
  rectangle->x = x + _offsetX;
  rectangle->y = y + _offsetY;
}

void Texture::setOffset(short x, short y) {
  _offsetX = x;
  _offsetY = y;
  rectangle->x += _offsetX;
  rectangle->y += _offsetY;
}

void Texture::setText(const std::string &_text) { text = _text; }

short Texture::getWidth() {
  switch (type) {
  case IMAGE:
    return (short)rectangle->w;
  case FONT:
    return (short)FC_GetWidth(font, text.c_str());
  case NONE:
    return 0;
  }
}

short Texture::getHeight() {
  switch (type) {
  case IMAGE:
    return (short)rectangle->h;
  case FONT:
    return (short)FC_GetHeight(font, text.c_str());
  case NONE:
    return 0;
  }
}

void Texture::center() { setOffset(-(getWidth() / 2), -(getHeight() / 2)); }
