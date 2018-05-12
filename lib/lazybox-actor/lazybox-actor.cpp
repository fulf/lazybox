#include "lazybox-actor.h"

LazyBoxActor::LazyBoxActor(LazyBoxCore* core) {
  _core = core;
}

void LazyBoxActor::act(String pin, String action) {
  LazyBoxPin p = _core->getPin(pin);

  _core->setPinOutput(p, action == "on" ? 1 : 0);
}
