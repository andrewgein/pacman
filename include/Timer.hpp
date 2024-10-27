#ifndef __TIMER_H__
#define __TIMER_H__

#include <SDL2/SDL.h>
#include <algorithm>

class Timer {
public:
  Timer(int);
  void start();
  bool isTriggered();
  bool isActive();
  void deactivate();
  void pause();
  void resume();

private:
  int _startTicks;
  int _triggerTime;
  bool _paused;
  bool _active;
};

#endif
