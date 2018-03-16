

#include <X11/Xatom.h>
#include <X11/cursorfont.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <gdk/gdkx.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "misc.h"
#include "panel.h"
#include "gdk-helper.h"

//#define DEBUG
#include "dbg.h"


/* X11 data types */
Atom a_UTF8_STRING;
Atom a_XROOTPMAP_ID;

/* old WM spec */
Atom a_WM_STATE;
Atom a_WM_CLASS;

/* new NET spec */
Atom a_NET_WORKAREA;
Atom a_NET_CLIENT_LIST;
Atom a_NET_CLIENT_LIST_STACKING;
Atom a_NET_NUMBER_OF_DESKTOPS;
Atom a_NET_CURRENT_DESKTOP;
Atom a_NET_DESKTOP_NAMES;
Atom a_NET_ACTIVE_WINDOW;
Atom a_NET_WM_DESKTOP;
Atom a_NET_WM_STATE;
Atom a_NET_WM_STATE_SKIP_TASKBAR;
Atom a_NET_WM_STATE_SKIP_PAGER;
Atom a_NET_WM_STATE_STICKY;
Atom a_NET_WM_STATE_HIDDEN;
Atom a_NET_WM_STATE_SHADED;
Atom a_NET_WM_WINDOW_TYPE;
Atom a_NET_WM_WINDOW_TYPE_DESKTOP;
Atom a_NET_WM_WINDOW_TYPE_DOCK;
Atom a_NET_WM_WINDOW_TYPE_TOOLBAR;
Atom a_NET_WM_WINDOW_TYPE_MENU;
Atom a_NET_WM_WINDOW_TYPE_UTILITY;
Atom a_NET_WM_WINDOW_TYPE_SPLASH;
Atom a_NET_WM_WINDOW_TYPE_DIALOG;
Atom a_NET_WM_WINDOW_TYPE_NORMAL;
Atom a_NET_WM_DESKTOP;
Atom a_NET_WM_NAME;
Atom a_NET_WM_STRUT;
Atom a_NET_WM_STRUT_PARTIAL;
Atom a_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR;

pair allign_pair[] = {
    { ALLIGN_NONE, "none" },
    { ALLIGN_LEFT, "left" },
    { ALLIGN_RIGHT, "right" },
    { ALLIGN_CENTER, "center"},
    { 0, NULL },
};

pair edge_pair[] = {
    { EDGE_NONE, "none" },
    { EDGE_LEFT, "left" },
    { EDGE_RIGHT, "right" },
    { EDGE_TOP, "top" },
    { EDGE_BOTTOM, "bottom" },
    { 0, NULL },
};

pair distancefrom_pair[] = {
    { DISTANCEFROM_NONE, "none" },
    { DISTANCEFROM_LEFT, "left" },
    { DISTANCEFROM_RIGHT, "right" },
    { DISTANCEFROM_TOP, "top" },
    { DISTANCEFROM_BOTTOM, "bottom" },
    { 0, NULL },
};

pair width_pair[] = {
    { WIDTH_NONE, "none" },
    { WIDTH_REQUEST, "request" },
    { WIDTH_PIXEL, "pixel" },
    { WIDTH_PERCENT, "percent" },
    { 0, NULL },
};

pair height_pair[] = {
    { HEIGHT_NONE, "none" },
    { HEIGHT_REQUEST, "request" },
    { HEIGHT_PIXEL, "pixel" },
    { 0, NULL },
};

pair bool_pair[] = {
    { 0, "false" },
    { 1, "true" },
    { 0, NULL },
};
pair pos_pair[] = {
    { POS_NONE, "none" },
    { POS_START, "start" },
    { POS_END,  "end" },
    { 0, NULL},
};


int
str2num(pair *p, gchar *str, int defval)
{
    ENTER;
    for (;p && p->str; p++) {
        if (!g_ascii_strcasecmp(str, p->str))
            RET(p->num);
    }
    RET(defval);
}

void resolve_atoms()
{
    ENTER;

    a_UTF8_STRING                = XInternAtom(gdk_helper_display(), "UTF8_STRING", False);
    a_XROOTPMAP_ID               = XInternAtom(gdk_helper_display(), "_XROOTPMAP_ID", False);
    a_WM_STATE                   = XInternAtom(gdk_helper_display(), "WM_STATE", False);
    a_WM_CLASS                   = XInternAtom(gdk_helper_display(), "WM_CLASS", False);
    a_NET_WORKAREA               = XInternAtom(gdk_helper_display(), "_NET_WORKAREA", False);
    a_NET_CLIENT_LIST            = XInternAtom(gdk_helper_display(), "_NET_CLIENT_LIST", False);
    a_NET_CLIENT_LIST_STACKING   = XInternAtom(gdk_helper_display(), "_NET_CLIENT_LIST_STACKING", False);
    a_NET_NUMBER_OF_DESKTOPS     = XInternAtom(gdk_helper_display(), "_NET_NUMBER_OF_DESKTOPS", False);
    a_NET_CURRENT_DESKTOP        = XInternAtom(gdk_helper_display(), "_NET_CURRENT_DESKTOP", False);
    a_NET_DESKTOP_NAMES          = XInternAtom(gdk_helper_display(), "_NET_DESKTOP_NAMES", False);
    a_NET_ACTIVE_WINDOW          = XInternAtom(gdk_helper_display(), "_NET_ACTIVE_WINDOW", False);
    a_NET_WM_DESKTOP             = XInternAtom(gdk_helper_display(), "_NET_WM_DESKTOP", False);
    a_NET_WM_STATE               = XInternAtom(gdk_helper_display(), "_NET_WM_STATE", False);
    a_NET_WM_STATE_SKIP_TASKBAR  = XInternAtom(gdk_helper_display(), "_NET_WM_STATE_SKIP_TASKBAR", False);
    a_NET_WM_STATE_SKIP_PAGER    = XInternAtom(gdk_helper_display(), "_NET_WM_STATE_SKIP_PAGER", False);
    a_NET_WM_STATE_STICKY        = XInternAtom(gdk_helper_display(), "_NET_WM_STATE_STICKY", False);
    a_NET_WM_STATE_HIDDEN        = XInternAtom(gdk_helper_display(), "_NET_WM_STATE_HIDDEN", False);
    a_NET_WM_STATE_SHADED        = XInternAtom(gdk_helper_display(), "_NET_WM_STATE_SHADED", False);
    a_NET_WM_WINDOW_TYPE         = XInternAtom(gdk_helper_display(), "_NET_WM_WINDOW_TYPE", False);

    a_NET_WM_WINDOW_TYPE_DESKTOP = XInternAtom(gdk_helper_display(), "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    a_NET_WM_WINDOW_TYPE_DOCK    = XInternAtom(gdk_helper_display(), "_NET_WM_WINDOW_TYPE_DOCK", False);
    a_NET_WM_WINDOW_TYPE_TOOLBAR = XInternAtom(gdk_helper_display(), "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
    a_NET_WM_WINDOW_TYPE_MENU    = XInternAtom(gdk_helper_display(), "_NET_WM_WINDOW_TYPE_MENU", False);
    a_NET_WM_WINDOW_TYPE_UTILITY = XInternAtom(gdk_helper_display(), "_NET_WM_WINDOW_TYPE_UTILITY", False);
    a_NET_WM_WINDOW_TYPE_SPLASH  = XInternAtom(gdk_helper_display(), "_NET_WM_WINDOW_TYPE_SPLASH", False);
    a_NET_WM_WINDOW_TYPE_DIALOG  = XInternAtom(gdk_helper_display(), "_NET_WM_WINDOW_TYPE_DIALOG", False);
    a_NET_WM_WINDOW_TYPE_NORMAL  = XInternAtom(gdk_helper_display(), "_NET_WM_WINDOW_TYPE_NORMAL", False);
    a_NET_WM_DESKTOP             = XInternAtom(gdk_helper_display(), "_NET_WM_DESKTOP", False);
    a_NET_WM_NAME                = XInternAtom(gdk_helper_display(), "_NET_WM_NAME", False);
    a_NET_WM_STRUT               = XInternAtom(gdk_helper_display(), "_NET_WM_STRUT", False);
    a_NET_WM_STRUT_PARTIAL       = XInternAtom(gdk_helper_display(), "_NET_WM_STRUT_PARTIAL", False);
    a_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR
                                 = XInternAtom(gdk_helper_display(), "_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR", False);

    RET();
}


void
Xclimsg(Window win, long type, long l0, long l1, long l2, long l3, long l4)
{
    XClientMessageEvent xev;

    xev.type = ClientMessage;
    xev.window = win;
    xev.message_type = type;
    xev.format = 32;
    xev.data.l[0] = l0;
    xev.data.l[1] = l1;
    xev.data.l[2] = l2;
    xev.data.l[3] = l3;
    xev.data.l[4] = l4;
    XSendEvent(gdk_helper_display(), GDK_ROOT_WINDOW(), False,
          (SubstructureNotifyMask | SubstructureRedirectMask),
          (XEvent *) & xev);
}


static void
calculate_width(int scrw, int wtype, int allign, int margin,
      int *panw, int *x)
{
    ENTER;
    DBG("scrw=%d\n", scrw);
    DBG("IN panw=%d\n", *panw);
    //scrw -= 2;
    if (wtype == WIDTH_PERCENT) {
        /* sanity check */
        if (*panw > 100)
            *panw = 100;
        else if (*panw < 0)
            *panw = 1;
        *panw = ((gfloat) scrw * (gfloat) *panw) / 100.0;
    }
    if (allign != ALLIGN_CENTER) {
        if (margin > scrw) {
            ERR( "margin is bigger then edge size %d > %d. Ignoring margin\n",
                  margin, scrw);
            margin = 0;
        }
        if (wtype == WIDTH_PERCENT)
            //*panw = MAX(scrw - margin, *panw);
            ;
        else
            *panw = MIN(scrw - margin, *panw);
    }
    DBG("OUT panw=%d\n", *panw);
    if (allign == ALLIGN_LEFT)
        *x += margin;
    else if (allign == ALLIGN_RIGHT) {
        *x += scrw - *panw - margin;
        if (*x < 0)
            *x = 0;
    } else if (allign == ALLIGN_CENTER)
        *x += (scrw - *panw) / 2;
    RET();
}


void
calculate_position(panel *np, int distance,int distancefrom)
{
    int sswidth, ssheight, minx, miny, distancex, distancey;
    GdkScreen *screen;
    GdkDisplay *display;
    GdkRectangle *monitorGeometry;

    ENTER;
    
    display = gdk_display_get_default ();
    screen  = gdk_display_get_default_screen(display);

    monitorGeometry = (GdkRectangle*) malloc(sizeof(GdkRectangle));

    if (np->monitor >= gdk_screen_get_n_monitors(screen)) {
        np->monitor = 0;
        ERR("trayer: monitor parameter isn't valid, resetting value to 0\n");
    }

    gdk_screen_get_monitor_geometry(screen,np->monitor,monitorGeometry);
    
    sswidth  = monitorGeometry->width; 
    ssheight = monitorGeometry->height;

    minx = monitorGeometry->x;
    miny = monitorGeometry->y;

    free(monitorGeometry);
    
    if (distancefrom == DISTANCEFROM_TOP || distancefrom == DISTANCEFROM_BOTTOM) {
        distancex = 0;
        distancey = (distancefrom == DISTANCEFROM_TOP) ? distance : -distance;
    } else {
        distancex = (distancefrom == DISTANCEFROM_LEFT) ? distance : -distance;
        distancey = 0;
    }

    if (np->edge == EDGE_TOP || np->edge == EDGE_BOTTOM) {
        np->aw = np->width;
        np->ax = minx + distancex;
        calculate_width(sswidth, np->widthtype, np->allign, np->margin,
              &np->aw, &np->ax);
        np->ah = np->height;
        np->ah = MIN(PANEL_HEIGHT_MAX, np->ah);
        np->ah = MAX(PANEL_HEIGHT_MIN, np->ah);
        np->ay = miny + ((np->edge == EDGE_TOP) ? distancey : (ssheight - np->ah - distancey));

    } else {
        np->ah = np->width;
        np->ay = miny + distancey;
        calculate_width(ssheight, np->widthtype, np->allign, np->margin,
              &np->ah, &np->ay);
        np->aw = np->height;
        np->aw = MIN(PANEL_HEIGHT_MAX, np->aw);
        np->aw = MAX(PANEL_HEIGHT_MIN, np->aw);
        np->ax = minx + ((np->edge == EDGE_LEFT) ? distancex : (sswidth - np->aw - distancex));
    }
    DBG("%s - x=%d y=%d w=%d h=%d\n", __FUNCTION__, np->ax, np->ay, np->aw, np->ah);
    RET();
}
