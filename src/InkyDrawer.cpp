#include "InkyDrawer.hpp"

InkyDrawer::InkyDrawer(const GhostData &data, SDL_Renderer *renderer,
                       const point_t &mapOffset)
    : GhostDrawer(data, renderer, {INKY_TEXTURE_OFFSET_X, INKY_TEXTURE_OFFSET_Y},
                  mapOffset) {}
