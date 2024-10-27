#ifndef __INKY_DRAWER_H__
#define __INKY_DRAWER_H__

#include "GhostDrawer.hpp"

class InkyDrawer : public GhostDrawer {
public:
  InkyDrawer(const GhostData &, SDL_Renderer *, const point_t &);
};

#endif
