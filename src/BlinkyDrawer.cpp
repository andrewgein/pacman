#include "BlinkyDrawer.hpp"

BlinkyDrawer::BlinkyDrawer(const GhostData &data, SDL_Renderer *renderer,
                           const point_t &mapOffset)
    : GhostDrawer(data, renderer, {BLINKY_TEXTURE_OFFSET_X, BLINKY_TEXTURE_OFFSET_Y},
                  mapOffset) {}
