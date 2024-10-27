#ifndef __BLINKY_DRAWER_H__
#define __BLINKY_DRAWER_H__

#include "GhostDrawer.hpp"
#include "Global.hpp"

class BlinkyDrawer : public GhostDrawer {
public:
  BlinkyDrawer(const GhostData &, SDL_Renderer *, const point_t &);
};

#endif
