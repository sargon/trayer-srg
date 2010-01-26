#ifndef _BG_H_
#define _BG_H_

#include <X11/X.h>

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <glib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

void bg_init();
void bg_rootbg_changed();
GdkPixmap *bg_new_for_win(Window win);
void modify_drawable(GdkDrawable *base, GdkGC *gc, guint32 tintcolor, gint alpha);

#endif

