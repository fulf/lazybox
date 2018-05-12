#ifndef LAZYBOX_ACTOR_H
#define LAZYBOX_ACTOR_H

#include <lazybox-core.h>

class LazyBoxActor{
  private:
    LazyBoxCore* _core;
  public:
    LazyBoxActor(LazyBoxCore*);
    void act(String, String);
};

#endif
