#include "ClydeDrawer.hpp"

ClydeDrawer::ClydeDrawer(const GhostData &data, SDL_Renderer *renderer,
                         const point_t &mapOffset)
    : GhostDrawer(data, renderer, {CLYDE_TEXTURE_OFFSET_X, CLYDE_TEXTURE_OFFSET_Y},
                  mapOffset) {}
