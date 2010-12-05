#include <gdk/gdkx.h>
#include <gdk/gdk.h>


#define G_NET_WM_STATE_REMOVE        0    /* remove/unset property */
#define G_NET_WM_STATE_ADD           1    /* add/set property */
#define G_NET_WM_STATE_TOGGLE        2    /* toggle property  */

#define TYPE_ATOM                 _GDK_MAKE_ATOM(4)
#define TYPE_CARDINAL             _GDK_MAKE_ATOM(6)
#define TYPE_PIXMAP               _GDK_MAKE_ATOM(20)


GdkAtom G_UTF8_STRING;
GdkAtom G_XROOTPMAP_ID;
GdkAtom G_WM_STATE;
GdkAtom G_WM_CLASS;
GdkAtom G_NET_WORKAREA;
GdkAtom G_NET_CLIENT_LIST;
GdkAtom G_NET_CLIENT_LIST_STACKING;
GdkAtom G_NET_NUMBER_OF_DESKTOPS;
GdkAtom G_NET_CURRENT_DESKTOP;
GdkAtom G_NET_DESKTOP_NAMES;
GdkAtom G_NET_ACTIVE_WINDOW;
GdkAtom G_NET_WM_STATE;
GdkAtom G_NET_WM_STATE_SKIP_TASKBAR;
GdkAtom G_NET_WM_STATE_SKIP_PAGER;
GdkAtom G_NET_WM_STATE_STICKY;
GdkAtom G_NET_WM_STATE_HIDDEN;
GdkAtom G_NET_WM_STATE_SHADED;
GdkAtom G_NET_WM_WINDOW_TYPE;
GdkAtom G_NET_WM_WINDOW_TYPE_DESKTOP;
GdkAtom G_NET_WM_WINDOW_TYPE_DOCK;
GdkAtom G_NET_WM_WINDOW_TYPE_TOOLBAR;
GdkAtom G_NET_WM_WINDOW_TYPE_MENU;
GdkAtom G_NET_WM_WINDOW_TYPE_UTILITY;
GdkAtom G_NET_WM_WINDOW_TYPE_SPLASH;
GdkAtom G_NET_WM_WINDOW_TYPE_DIALOG;
GdkAtom G_NET_WM_WINDOW_TYPE_NORMAL;
GdkAtom G_NET_WM_DESKTOP;
GdkAtom G_NET_WM_NAME;
GdkAtom G_NET_WM_STRUT;
GdkAtom G_NET_WM_STRUT_PARTIAL;
GdkAtom G_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR;

Display* gdk_helper_display();
GdkAtom  gdk_helper_atom_from_name(const gchar *atom_name);
void     gdk_helper_resolve_atoms();
