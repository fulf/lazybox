#include <lazybox.h>

LazyBox *lbc;

void setup() {
  lbc = new LazyBox(LazyBoxCore::WEMOSD1miniPro);
}

void loop(){
  lbc->run();
}
