#include "PinkyDrawer.hpp"

PinkyDrawer::PinkyDrawer(const GhostData &data, SDL_Renderer *renderer,
                         const point_t &mapOffset)
    : GhostDrawer(data, renderer, {PINKY_TEXTURE_OFFSET_X, PINKY_TEXTURE_OFFSET_Y},
                  mapOffset) {}
