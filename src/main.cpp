#include <lazybox-core.h>

LazyBoxCore *lbc;

void setup() {
  lbc = new LazyBoxCore();
}

void loop(){
  lbc->run();
}
