#ifndef __PINKY_DRAWER_H__
#define __PINKY_DRAWER_H__

#include "GhostDrawer.hpp"
#include "Global.hpp"

class PinkyDrawer : public GhostDrawer {
public:
  PinkyDrawer(const GhostData &, SDL_Renderer *, const point_t &);
};

#endif
