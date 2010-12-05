#include <gdk/gdkx.h>

#include "gdk-helper.h"

Display* gdk_helper_display() {                                                  
  return gdk_x11_display_get_xdisplay(gdk_display_get_default());
}

GdkAtom gdk_helper_atom_from_name(const gchar *atom_name){
  return gdk_x11_xatom_to_atom(gdk_x11_get_xatom_by_name(atom_name));
}

void gdk_helper_resolve_atoms() {


   G_UTF8_STRING = gdk_helper_atom_from_name("_UTF8_STRING");;
   G_XROOTPMAP_ID = gdk_helper_atom_from_name("_XROOTPMAP_ID");;

   G_WM_STATE = gdk_helper_atom_from_name("_WM_STATE");;
   G_WM_CLASS = gdk_helper_atom_from_name("_WM_CLASS");;

   G_NET_WORKAREA = gdk_helper_atom_from_name("_NET_WORKAREA");;
   G_NET_CLIENT_LIST = gdk_helper_atom_from_name("_NET_CLIENT_LIST");;
   G_NET_CLIENT_LIST_STACKING = gdk_helper_atom_from_name("_NET_CLIENT_LIST_STACKING");;
   G_NET_NUMBER_OF_DESKTOPS = gdk_helper_atom_from_name("_NET_NUMBER_OF_DESKTOPS");;
   G_NET_CURRENT_DESKTOP = gdk_helper_atom_from_name("_NET_CURRENT_DESKTOP");;
   G_NET_DESKTOP_NAMES = gdk_helper_atom_from_name("_NET_DESKTOP_NAMES");;
   G_NET_ACTIVE_WINDOW = gdk_helper_atom_from_name("_NET_ACTIVE_WINDOW");;
   G_NET_WM_STATE = gdk_helper_atom_from_name("_NET_WM_STATE");;
   G_NET_WM_STATE_SKIP_TASKBAR = gdk_helper_atom_from_name("_NET_WM_STATE_SKIP_TASKBAR");;
   G_NET_WM_STATE_SKIP_PAGER = gdk_helper_atom_from_name("_NET_WM_STATE_SKIP_PAGER");;
   G_NET_WM_STATE_STICKY = gdk_helper_atom_from_name("_NET_WM_STATE_STICKY");;
   G_NET_WM_STATE_HIDDEN = gdk_helper_atom_from_name("_NET_WM_STATE_HIDDEN");;
   G_NET_WM_STATE_SHADED = gdk_helper_atom_from_name("_NET_WM_STATE_SHADED");;

   G_NET_WM_WINDOW_TYPE = gdk_helper_atom_from_name("_NET_WM_WINDOW_TYPE");;
   G_NET_WM_WINDOW_TYPE_DESKTOP = gdk_helper_atom_from_name("_NET_WM_WINDOW_TYPE_DESKTOP");;
   G_NET_WM_WINDOW_TYPE_DOCK = gdk_helper_atom_from_name("_NET_WM_WINDOW_TYPE_DOCK");
   G_NET_WM_WINDOW_TYPE_TOOLBAR = gdk_helper_atom_from_name("_NET_WM_WINDOW_TYPE_TOOLBAR");;
   G_NET_WM_WINDOW_TYPE_MENU = gdk_helper_atom_from_name("_NET_WM_WINDOW_TYPE_MENU");;
   G_NET_WM_WINDOW_TYPE_UTILITY = gdk_helper_atom_from_name("_NET_WM_WINDOW_TYPE_UTILITY");;
   G_NET_WM_WINDOW_TYPE_SPLASH = gdk_helper_atom_from_name("_NET_WM_WINDOW_TYPE_SPLASH");;
   G_NET_WM_WINDOW_TYPE_DIALOG = gdk_helper_atom_from_name("_NET_WM_WINDOW_TYPE_DIALOG");;
   G_NET_WM_WINDOW_TYPE_NORMAL = gdk_helper_atom_from_name("_NET_WM_WINDOW_TYPE_NORMAL");;
           
   G_NET_WM_DESKTOP = gdk_helper_atom_from_name("_NET_WM_DESKTOP");;
   G_NET_WM_NAME = gdk_helper_atom_from_name("_NET_WM_NAME");;
   G_NET_WM_STRUT = gdk_helper_atom_from_name("_NET_WM_STRUT");;
   G_NET_WM_STRUT_PARTIAL = gdk_helper_atom_from_name("_NET_WM_STRUT_PARTIAL");;

   G_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR = gdk_helper_atom_from_name("_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR");;
}

