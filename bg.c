
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkpixmap.h>
#include <gdk/gdkprivate.h>
#include "bg.h"
#include "dbg.h"
#include "config.h"

static Display *dpy;
//static int screen;
static Window xroot;
static Pixmap xrootbg = 0xc046ad;
static GC gc;
static Atom id = 0;

static Pixmap
bg_get_root_bg()
{
  
    Pixmap ret;

    ENTER;
    ret = None;    
    if (id) {
        int  act_format, c = 2 ;
        u_long  nitems ;
        u_long  bytes_after ;
        u_char *prop ;
        Atom dummy_id;

        do {
            if (XGetWindowProperty(dpy, xroot, id, 0, 1,
                      False, XA_PIXMAP, &dummy_id, &act_format,
                      &nitems, &bytes_after, &prop) == Success) {
                if (prop) {
                    ret = *((Pixmap *)prop);
                    XFree(prop);
                    break;
                }
            }
        } while (--c > 0);
    }
    //fprintf(stderr, "_XROOTPMAP_ID = 0x%x\n", ret);
    RET(ret);
}

GdkPixmap *
bg_new_for_win(Window win)
{
    Window dummy;
    Pixmap bgpix;
    GdkPixmap *gbgpix;
    int  width, height ;
    int  border, depth, x, y ;

    ENTER;
    XGetGeometry(dpy, win, &dummy, &x, &y, &width, &height, &border, &depth);
    XTranslateCoordinates(dpy, win, xroot, 0, 0, &x, &y, &dummy);
    gbgpix = gdk_pixmap_new(NULL, width, height, depth);
    if (!gbgpix)
        RET(NULL);
    bgpix =  gdk_x11_drawable_get_xid(gbgpix);
    XSetTSOrigin(dpy, gc, -x, -y) ;
    XFillRectangle(dpy, bgpix, gc, 0, 0, width, height);
    RET(gbgpix);
}

void
bg_init(Display *dpyn)
{
    XGCValues  gcv;
    uint mask;
    
    ENTER;
    dpy = dpyn;
    id = XInternAtom(dpy, "_XROOTPMAP_ID", False);
    xroot = DefaultRootWindow(dpy);
    xrootbg = bg_get_root_bg();
   
    gcv.ts_x_origin = 0;
    gcv.ts_y_origin = 0;
    gcv.fill_style = FillTiled;
    mask = GCTileStipXOrigin | GCTileStipYOrigin | GCFillStyle;
    if (xrootbg) {
        gcv.tile = xrootbg;
        mask |= GCTile ;
    }
    gc = XCreateGC (dpy, xroot, mask, &gcv) ;
    RET();
}

void
bg_rootbg_changed()
{
    ENTER;
    xrootbg = bg_get_root_bg();
    if (xrootbg != None) {
        XGCValues  gcv;
        
        gcv.tile = xrootbg;
        XChangeGC(dpy, gc, GCTile, &gcv);
        DBG("changed\n");
    }
    RET();
}

void
bg_close()
{
    ENTER;
    XFreeGC(dpy, gc);
    RET();
}

void
modify_drawable(GdkDrawable *base, GdkGC *gc, guint32 tintcolor, gint alpha)
{
    int w, h;
    GdkPixbuf *ret, *ret2;
    static GdkColormap *cmap = NULL;

    ENTER;
    gdk_drawable_get_size (base, &w, &h);
    if (!cmap) {
        cmap = gdk_colormap_get_system ();
    }
    ret = gdk_pixbuf_get_from_drawable (NULL, base, cmap, 0, 0, 0, 0, w, h);
    if (!ret)
        RET();
    ret2 = gdk_pixbuf_composite_color_simple(ret, w, h, 
          GDK_INTERP_HYPER, alpha, MIN(w, h), tintcolor, tintcolor);

    //gdk_pixbuf_render_to_drawable (ret2, base, gc, 0, 0, 0, 0, w, h, GDK_RGB_DITHER_NONE, 0, 0);
    gdk_draw_pixbuf (base, gc, ret2, 0, 0, 0, 0, w, h, GDK_RGB_DITHER_NONE, 0, 0);
    g_object_unref(ret);
    g_object_unref(ret2);
    RET();
}

#ifdef TRANSPARENCY

#include <gdk/gdkwindow.h>
//#include <gdk/gdkinternals.h>
#include <gdk/gdk.h>		/* For gdk_rectangle_union() */
#include <gdk/gdkpixmap.h>
#include <gdk/gdkdrawable.h>
#include <gdk/gdkpixmap.h>
#include <gdk/gdkscreen.h>



typedef struct _GdkWindowPaint GdkWindowPaint;

struct _GdkWindowPaint
{
    GdkRegion *region;
    GdkPixmap *pixmap;
    gint x_offset;
    gint y_offset;
};



GdkGC *
gdk_window_get_bg_gc (GdkWindow      *window,
		      GdkWindowPaint *paint)
{
    GdkWindowObject *private = (GdkWindowObject *)window;
    guint gc_mask = 0;
    GdkGCValues gc_values;

    ENTER;
    if (private->bg_pixmap == GDK_PARENT_RELATIVE_BG && private->parent) {
        GdkWindowPaint tmp_paint = *paint;
        tmp_paint.x_offset += private->x;
        tmp_paint.y_offset += private->y;
      
        return gdk_window_get_bg_gc (GDK_WINDOW (private->parent), &tmp_paint);
    } else if (private->bg_pixmap && 
          private->bg_pixmap != GDK_PARENT_RELATIVE_BG && 
          private->bg_pixmap != GDK_NO_BG) {
        gc_values.fill = GDK_TILED;
        gc_values.tile = private->bg_pixmap;
        gc_values.ts_x_origin = -paint->x_offset;
        gc_values.ts_y_origin = -paint->y_offset;
        gc_mask = GDK_GC_FILL | GDK_GC_TILE | GDK_GC_TS_X_ORIGIN | GDK_GC_TS_Y_ORIGIN;

        DBG("x_offset=%d y_offset=%d\n", paint->x_offset, paint->y_offset);
        return gdk_gc_new_with_values (paint->pixmap, &gc_values, gc_mask);
    } else {
        GdkGC *gc = _gdk_drawable_get_scratch_gc (paint->pixmap, FALSE);

        gdk_gc_set_foreground (gc, &(private->bg_color));

        return g_object_ref (gc);
    }
}

#endif
