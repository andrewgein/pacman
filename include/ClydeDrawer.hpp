#ifndef __CLYDE_DRAWER_H__
#define __CLYDE_DRAWER_H__

#include "GhostDrawer.hpp"

class ClydeDrawer : public GhostDrawer {
public:
  ClydeDrawer(const GhostData &, SDL_Renderer *, const point_t &);
};

#endif
