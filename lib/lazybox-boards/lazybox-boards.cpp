#include "lazybox-boards.h"

namespace WEMOSD1miniPro {
  int PinCount = 10;
  LazyBoxPin Pins[] = {
    (LazyBoxPin){"D0", D0, true, "input"},
    (LazyBoxPin){"D1", D1, true, "input"},
    (LazyBoxPin){"D2", D2, true, "input"},
    (LazyBoxPin){"D3", D3, true, "input"},
    (LazyBoxPin){"D4", D4, true, "input"},
    (LazyBoxPin){"D5", D5, true, "input"},
    (LazyBoxPin){"D6", D6, true, "input"},
    (LazyBoxPin){"D7", D7, true, "input"},
    (LazyBoxPin){"D8", D8, true, "input"},
    (LazyBoxPin){"A0", A0, false, "input"}
  };
}
