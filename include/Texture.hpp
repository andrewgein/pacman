#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <SDL2/SDL.h>
#include <SDL_FontCache.h>
#include <SDL_image.h>
#include <string>

#include "Drawable.hpp"
#include "Global.hpp"
#include "Point.h"

class TextureException : public std::exception {
public:
  char *what() { return (char *)"texture load erorr"; }
};

class Texture : public Drawable {
public:
  Texture();
  int loadImage(const std::string &file, SDL_Renderer *_renderer, SDL_Rect *clip);
  int loadImage(const std::string &file, SDL_Renderer *_renderer, const int &clipX,
                const int &clipY, const int &clipW, const int &clipH);

  int loadFont(const std::string &file, int fontSize, const SDL_Color *color,
               SDL_Renderer *_renderer);
  int loadFont(const std::string &file, const std::string &_text, int fontSize,
               const SDL_Color *color, SDL_Renderer *_renderer);

  void draw(int, int) const;
  void draw(point_t) const;
  void draw() const;
  void setCoordinates(int x, int y);
  void setOffset(short, short);
  void setText(const std::string &_file);
  short getWidth();
  short getHeight();
  void center();

private:
  enum Type { IMAGE, FONT } type;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  SDL_Rect *rectangle;
  SDL_Rect *clip;

  short _offsetX, _offsetY;
  std::string text;
  FC_Font *font;
};

#endif
