#include "Timer.hpp"

Timer::Timer(int triggerTime) {
  _triggerTime = triggerTime;
  _startTicks = 0;
  _active = false;
  _paused = false;
}

void Timer::start() {
  _startTicks = SDL_GetTicks();
  _active = true;
}

bool Timer::isTriggered() {
  if (_active) {
    return SDL_GetTicks() - _startTicks >= _triggerTime;
  }
  return false;
}

void Timer::deactivate() { _active = false; }

void Timer::pause() {
  assert(_active);
  _triggerTime -= std::min((Uint32)SDL_GetTicks() - _startTicks, (Uint32)_triggerTime);
  _active = false;
}

bool Timer::isActive() { return _active; }
