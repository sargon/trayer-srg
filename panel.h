#ifndef PANEL_H
#define PANEL_H


#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "config.h"

enum { ALLIGN_NONE, ALLIGN_LEFT, ALLIGN_RIGHT, ALLIGN_CENTER };
enum { EDGE_NONE, EDGE_LEFT, EDGE_RIGHT, EDGE_TOP, EDGE_BOTTOM };
enum { DISTANCEFROM_NONE, DISTANCEFROM_LEFT, DISTANCEFROM_RIGHT, DISTANCEFROM_TOP, DISTANCEFROM_BOTTOM };
enum { WIDTH_NONE, WIDTH_REQUEST, WIDTH_PIXEL, WIDTH_PERCENT };
enum { HEIGHT_NONE, HEIGHT_PIXEL, HEIGHT_REQUEST };
enum { ORIENT_NONE, ORIENT_VERT, ORIENT_HORIZ };
enum { POS_NONE, POS_START, POS_END };

#define PANEL_HEIGHT_DEFAULT  26
#define PANEL_HEIGHT_MAX      200
#define PANEL_HEIGHT_MIN      2
#define PANEL_WIDTH_DEFAULT   100

typedef struct {

    GtkWidget *topgwin;           /* main panel window */
    GdkWindow* topGdkWindow;      // and it parent gdk window  
    Window topxwin;               /* and it X window   */
    GtkWidget *lbox;              /* primary layout box */
    GtkWidget *box;               /* box that contains all plugins */
    GtkWidget *(*my_box_new) (gboolean, gint);
    Pixmap xtopbg; 
    GdkPixmap *gtopbg;

    int alpha;
    guint32 tintcolor;
    
    int ax, ay, aw, ah;  /* actual location and size of a panel */
    int allign, edge, margin;
    int orientation;
    int widthtype, width;
    int heighttype, height;

    int self_destroy : 1;
    int setdocktype : 1;
    int setstrut : 1;
    int transparent : 1;
    int on_primary : 1;
    int monitor;
    
    // Import from plugin structure
    gpointer     priv; 
    int expand;
    int padding;
} panel;


typedef struct { 
    unsigned int modal : 1;
    unsigned int sticky : 1;
    unsigned int maximized_vert : 1;
    unsigned int maximized_horz : 1;
    unsigned int shaded : 1;
    unsigned int skip_taskbar : 1;
    unsigned int skip_pager : 1;
    unsigned int hidden : 1;
    unsigned int fullscreen : 1;
    unsigned int above : 1;
    unsigned int below : 1;
} net_wm_state;

typedef struct { 
    unsigned int desktop : 1;
    unsigned int dock : 1;
    unsigned int toolbar : 1;
    unsigned int menu : 1;
    unsigned int utility : 1;
    unsigned int splash : 1;
    unsigned int dialog : 1;
    unsigned int normal : 1;
} net_wm_window_type;


extern Atom a_UTF8_STRING;
extern Atom a_XROOTPMAP_ID;

extern Atom a_WM_STATE;
extern Atom a_WM_CLASS;

extern Atom a_NET_WORKAREA;
extern Atom a_NET_CLIENT_LIST;
extern Atom a_NET_CLIENT_LIST_STACKING;
extern Atom a_NET_NUMBER_OF_DESKTOPS;
extern Atom a_NET_CURRENT_DESKTOP;
extern Atom a_NET_DESKTOP_NAMES;
extern Atom a_NET_ACTIVE_WINDOW;
extern Atom a_NET_WM_STATE;
extern Atom a_NET_WM_STATE_SKIP_TASKBAR;
extern Atom a_NET_WM_STATE_SKIP_PAGER;
extern Atom a_NET_WM_STATE_STICKY;
extern Atom a_NET_WM_STATE_HIDDEN;
extern Atom a_NET_WM_STATE_SHADED;

#define a_NET_WM_STATE_REMOVE        0    /* remove/unset property */
#define a_NET_WM_STATE_ADD           1    /* add/set property */
#define a_NET_WM_STATE_TOGGLE        2    /* toggle property  */

extern Atom a_NET_WM_WINDOW_TYPE;
extern Atom a_NET_WM_WINDOW_TYPE_DESKTOP;
extern Atom a_NET_WM_WINDOW_TYPE_DOCK;
extern Atom a_NET_WM_WINDOW_TYPE_TOOLBAR;
extern Atom a_NET_WM_WINDOW_TYPE_MENU;
extern Atom a_NET_WM_WINDOW_TYPE_UTILITY;
extern Atom a_NET_WM_WINDOW_TYPE_SPLASH;
extern Atom a_NET_WM_WINDOW_TYPE_DIALOG;
extern Atom a_NET_WM_WINDOW_TYPE_NORMAL;
          
extern Atom a_NET_WM_DESKTOP;
extern Atom a_NET_WM_NAME;
extern Atom a_NET_WM_STRUT;
extern Atom a_NET_WM_STRUT_PARTIAL;

extern Atom a_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR;
  
#endif
