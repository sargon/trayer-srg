#include <gdk/gdkx.h>

#include "gdk-helper.h"

Display* gdk_helper_display() {                                                  
  return gdk_x11_display_get_xdisplay(gdk_display_get_default());
}
