#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <gdk-pixbuf/gdk-pixbuf.h>

#include "panel.h"
#include "misc.h"

#include "eggtraymanager.h"
#include "fixedtip.h"
#include "main.h"

//#define DEBUG
#include "dbg.h"


typedef struct {
    GtkWidget *mainw;
    panel *panel;
    GtkWidget *box;
    /////
    EggTrayManager *tray_manager;
    
} tray;

static void
tray_added (EggTrayManager *manager, GtkWidget *icon, void *data)
{
    GtkWidget *box = (GtkWidget *)data;

    gtk_box_pack_end (GTK_BOX (box), icon, FALSE, FALSE, 0);
    gtk_widget_show (icon);
}

static void
tray_removed (EggTrayManager *manager, GtkWidget *icon, void *data)
{

}

static void
message_sent (EggTrayManager *manager, GtkWidget *icon, const char *text, glong id, glong timeout,
              void *data)
{
    /* FIXME multihead */
    int x, y;
    
    gdk_window_get_origin (icon->window, &x, &y);
  
    fixed_tip_show (0, x, y, FALSE, gdk_screen_height () - 50, text);
}

static void
message_cancelled (EggTrayManager *manager, GtkWidget *icon, glong id,
                   void *data)
{
  
}

void
tray_destructor(panel *p)
{
    tray *tr = (tray *)p->priv;

    ENTER;
    /* Make sure we drop the manager selection */
    g_object_unref (G_OBJECT (tr->tray_manager));
    fixed_tip_hide ();
    gtk_widget_destroy(tr->mainw);
    g_free(tr);
    RET();
}

int
tray_constructor(panel *p)
{
    tray *tr;
    GdkScreen *screen;
    
    ENTER;
    
    tr = g_new0(tray, 1);
    g_return_val_if_fail(tr != NULL, 0);
    p->priv = tr;
    tr->panel = p;
    tr->mainw = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);
    tr->box = p->my_box_new(FALSE, 1);
    gtk_box_set_spacing(GTK_BOX(tr->box), p->icon_spacing);
    gtk_container_add (GTK_CONTAINER (tr->mainw), tr->box);
    gtk_container_add(GTK_CONTAINER(p->box), tr->mainw);
    
    screen = gtk_widget_get_screen (GTK_WIDGET (p->topgwin));
    
    if (egg_tray_manager_check_running(screen)) {
        ERR("another systray already running\n");
        RET(1);
    }
    tr->tray_manager = egg_tray_manager_new ();

    if (!egg_tray_manager_manage_screen (tr->tray_manager, screen))
        g_printerr ("System tray didn't get the system tray manager selection\n");
    
    g_signal_connect (tr->tray_manager, "tray_icon_added",
          G_CALLBACK (tray_added), tr->box);
    g_signal_connect (tr->tray_manager, "tray_icon_removed",
          G_CALLBACK (tray_removed), tr->box);
    g_signal_connect (tr->tray_manager, "message_sent",
          G_CALLBACK (message_sent), tr->box);
    g_signal_connect (tr->tray_manager, "message_cancelled",
          G_CALLBACK (message_cancelled), tr->box);
    
    
    RET(1);

}
