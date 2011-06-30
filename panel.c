#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <locale.h>
#include <string.h>
#include <signal.h>

#include <gdk/gdk.h>

#include "panel.h"
#include "misc.h"
#include "bg.h"
#include "main.h"
#include "gdk-helper.h"

#define VERSION "1.1.1"

static gchar version[] = VERSION;
int distance=0, distancefrom=DISTANCEFROM_TOP;
int expand=1 , padding=0;


//#define DEBUG
#include "dbg.h"


static panel *p;
static gchar *transparent_rc = "style 'transparent-style'\n"
"{\n"
"bg_pixmap[NORMAL] = \"<parent>\"\n"
"bg_pixmap[INSENSITIVE] = \"<parent>\"\n"
"bg_pixmap[PRELIGHT] = \"<parent>\"\n"
"bg_pixmap[SELECTED] = \"<parent>\"\n"
"bg_pixmap[ACTIVE] = \"<parent>\"\n"
"}\n"
"class \"GtkEventBox\" style \"transparent-style\"\n"
"class \"GtkSocket\" style \"transparent-style\"\n"
"class \"GtkBar\" style \"transparent-style\"\n"
"class \"GtkBox\" style \"transparent-style\"\n"
"\n";

static void set_bg(GtkWidget *widget, panel *p);

/****************************************************
 *         panel's handlers for WM events           *
 ****************************************************/
/*
static void
panel_del_wm_strut(panel *p)
{
    XDeleteProperty(gdk_helper_display(), p->topxwin, a_NET_WM_STRUT);
    XDeleteProperty(gdk_helper_display(), p->topxwin, a_NET_WM_STRUT_PARTIAL);
}
*/

static void
panel_set_wm_strut(panel *p)
{
    unsigned long data[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int i = 4;

    ENTER;
    if (!GTK_WIDGET_MAPPED (p->topgwin))
        return;
    switch (p->edge) {
    case EDGE_LEFT:
        i = 0;
        data[i] = p->aw;
        data[4 + i*2] = p->ay;
        data[5 + i*2] = p->ay + p->ah - 1;
        break;
    case EDGE_RIGHT:
        i = 1;
        data[i] = p->aw;
        data[4 + i*2] = p->ay;
        data[5 + i*2] = p->ay + p->ah - 1;
        break;
    case EDGE_TOP:
        i = 2;
        data[i] = p->ah;
        data[4 + i*2] = p->ax;
        data[5 + i*2] = p->ax + p->aw - 1;
        break;
    case EDGE_BOTTOM:
        i = 3;
        data[i] = p->ah;
        data[4 + i*2] = p->ax;
        data[5 + i*2] = p->ax + p->aw - 1 ;
        break;
    default:
        ERR("wrong edge %d. strut won't be set\n", p->edge);
        RET();
    }		
    DBG("type %d. width %d. from %d to %d\n", i, data[i], data[4 + i*2], data[5 + i*2]);

    XChangeProperty(gdk_helper_display(), p->topxwin, a_NET_WM_STRUT_PARTIAL,
                    XA_CARDINAL, 32, PropModeReplace,  (unsigned char *) data, 12);
   /* old spec, for wms that do not support STRUT_PARTIAL */
    XChangeProperty(gdk_helper_display(), p->topxwin, a_NET_WM_STRUT,
                    XA_CARDINAL, 32, PropModeReplace,  (unsigned char *) data, 4);

    RET();
}

static GdkFilterReturn 
panel_wm_events(GdkXEvent *xevent, GdkEvent *event, panel *p)
{
    Atom at;
    Window win;
    XEvent *ev = (XEvent *) xevent;

    ENTER;
    DBG("win = 0x%x\n", ev->xproperty.window);
    if ( ev->type != PropertyNotify )
        RET(GDK_FILTER_CONTINUE);

    at = ev->xproperty.atom;
    win = ev->xproperty.window;
    if (win == GDK_ROOT_WINDOW()) {
      if (at == a_XROOTPMAP_ID) {
            bg_rootbg_changed();
            set_bg(p->topgwin, p);
            gtk_widget_queue_draw(p->topgwin);
            DBG("a_XROOTPMAP_ID\n");
      }
    }
    RET(GDK_FILTER_CONTINUE);
}

/****************************************************
 *         panel's handlers for GTK events          *
 ****************************************************/


static gint 
panel_delete_event(GtkWidget * widget, GdkEvent * event, gpointer data)
{
    ENTER;
    RET(FALSE);
}

static gint
panel_destroy_event(GtkWidget * widget, GdkEvent * event, gpointer data)
{
    ENTER;
    // TODO need to cleanup 
    gtk_main_quit();
    RET(FALSE);
}



static gint
panel_size_req(GtkWidget *widget, GtkRequisition *req, panel *p)
{
    ENTER;
    DBG("IN req=(%d, %d)\n", req->width, req->height);
    if (p->widthtype == WIDTH_REQUEST)
        p->width = (p->orientation == ORIENT_HORIZ) ? req->width : req->height;
    if (p->heighttype == HEIGHT_REQUEST)
        p->height = (p->orientation == ORIENT_HORIZ) ? req->height : req->width;
    calculate_position(p, distance,distancefrom);
    req->width  = p->aw;
    req->height = p->ah;
    DBG("OUT req=(%d, %d)\n", req->width, req->height);
    RET( TRUE );
}

static gint
panel_size_alloc(GtkWidget *widget, GtkAllocation *a, panel *p)
{
    ENTER;
    DBG("new alloc: size (%d, %d). pos (%d, %d)\n", a->width, a->height, a->x, a->y);
    DBG("old alloc: size (%d, %d). pos (%d, %d)\n", p->aw, p->ah, p->ax, p->ay);
    if (p->widthtype == WIDTH_REQUEST)
        p->width = (p->orientation == ORIENT_HORIZ) ? a->width : a->height;
    if (p->heighttype == HEIGHT_REQUEST)
        p->height = (p->orientation == ORIENT_HORIZ) ? a->height : a->width;
    calculate_position(p, distance,distancefrom);
    DBG("pref alloc: size (%d, %d). pos (%d, %d)\n", p->aw, p->ah, p->ax, p->ay);
    if (a->width == p->aw && a->height == p->ah && a->x == p->ax && a->y == p ->ay) {
        DBG("actual coords eq to preffered. just returning\n");
        RET(TRUE);
    }

    gtk_window_move(GTK_WINDOW(p->topgwin), p->ax, p->ay);
    if (p->setstrut)
        panel_set_wm_strut(p);
    RET(TRUE);
}




/****************************************************
 *         panel creation                           *
 ****************************************************/

  static void
set_bg(GtkWidget *widget, panel *p)
{
    ENTER;
    if (p->gtopbg)
        g_object_unref(p->gtopbg);
    p->gtopbg = bg_new_for_win(p->topxwin);

    modify_drawable(p->gtopbg, p->topgwin->style->black_gc, p->tintcolor, p->alpha);

    gdk_window_set_back_pixmap(p->topgwin->window, p->gtopbg, FALSE);
    gdk_window_clear(p->topgwin->window);
    gtk_widget_queue_draw_area (p->topgwin, 0, 0, 2000, 2000);
    RET();
}

static void
panel_style_set(GtkWidget *widget, GtkStyle *s, panel *p)
{
    ENTER;

    gtk_rc_parse_string(transparent_rc);
    if (GTK_WIDGET_REALIZED(widget))
        set_bg(widget, p);
    RET();
}

static gboolean
panel_configure_event(GtkWidget *widget, GdkEventConfigure *event, panel *p)
{
    static gint x = 0, y = 0, width = 0, height = 0;

    ENTER;
    if (x == event->x && y == event->y
          && width == event->width && height == event->height)
        RET(FALSE);
    x = event->x;
    y = event->y;
    width = event->width;
    height = event->height;
    set_bg(widget, p);
    RET(FALSE);
}

void
panel_start_gui(panel *p)
{
    ENTER;
    //gtk_rc_parse_string(transparent_rc);
    p->topgwin      = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_container_set_border_width(GTK_CONTAINER(p->topgwin), 0);
    gtk_window_set_resizable(GTK_WINDOW(p->topgwin), FALSE);
    gtk_window_set_wmclass(GTK_WINDOW(p->topgwin), "panel", "trayer");
    gtk_window_set_title(GTK_WINDOW(p->topgwin), "panel");
    g_signal_connect ( G_OBJECT(p->topgwin) , "delete-event" , G_CALLBACK(panel_delete_event) , p);
    g_signal_connect ( G_OBJECT(p->topgwin) , "destroy-event", G_CALLBACK(panel_destroy_event), p);
    g_signal_connect ( G_OBJECT (p->topgwin), "size-request" , G_CALLBACK(panel_size_req)  , p);
    g_signal_connect ( G_OBJECT (p->topgwin), "size-allocate", G_CALLBACK(panel_size_alloc), p);

    if (p->transparent) {
      g_signal_connect (G_OBJECT (p->topgwin), "configure-event", G_CALLBACK(panel_configure_event), p);
      g_signal_connect (G_OBJECT (p->topgwin), "style-set", G_CALLBACK( panel_style_set), p);
    } 
    gtk_widget_realize(p->topgwin);
    gdk_window_set_decorations(p->topgwin->window, 0);
    gtk_widget_set_app_paintable(p->topgwin, TRUE);

    p->lbox = p->my_box_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(p->lbox), 0);
    gtk_container_add(GTK_CONTAINER(p->topgwin), p->lbox);
    gtk_widget_show(p->lbox);

    if (p->allign == ALLIGN_RIGHT) {
        GtkWidget * expander = p->my_box_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(p->lbox), expander, TRUE, TRUE, 0);
        gtk_widget_show(expander);
    }

    p->box = p->my_box_new(FALSE, 1);
    gtk_container_set_border_width(GTK_CONTAINER(p->box), 1);
    gtk_box_pack_start(GTK_BOX(p->lbox), p->box, FALSE, TRUE, 0);
    gtk_widget_show(p->box);

    // get properties on topgwin
    p->topGdkWindow = gtk_widget_get_window(p->topgwin);
    p->topxwin = GDK_WINDOW_XWINDOW(GTK_WIDGET(p->topgwin)->window);

    bg_init(gdk_helper_display());

    /* make our window unfocusable */
    gdk_window_set_accept_focus(p->topGdkWindow,False);

    if (p->setdocktype) {
        gdk_window_set_type_hint(p->topGdkWindow,GDK_WINDOW_TYPE_HINT_DOCK);
    }

    Xclimsg(p->topxwin, a_NET_WM_DESKTOP, 0xFFFFFFFF, 0, 0, 0, 0);

    /************************/
    /* Window Mapping Point */
    gtk_widget_show_all(p->topgwin);
    Xclimsg(p->topxwin, a_NET_WM_DESKTOP, 0xFFFFFFFF, 0, 0, 0, 0);

    gdk_window_stick                 ( p->topGdkWindow);
    gdk_window_set_skip_pager_hint   ( p->topGdkWindow, True );
    gdk_window_set_skip_taskbar_hint ( p->topGdkWindow, True ); 

    XSelectInput (gdk_helper_display(), GDK_ROOT_WINDOW(), PropertyChangeMask);
    XSelectInput (gdk_helper_display(), p->topxwin, PropertyChangeMask | FocusChangeMask | StructureNotifyMask);
    gdk_window_add_filter(gdk_get_default_root_window (), (GdkFilterFunc)panel_wm_events, p);

    calculate_position(p, distance,distancefrom);
    gdk_window_move_resize(p->topgwin->window, p->ax, p->ay, p->aw, p->ah);
    if (p->setstrut)
        panel_set_wm_strut(p);


    RET();
}

static int
panel_parse_global(panel *p)
{
    ENTER;
    p->orientation = (p->edge == EDGE_TOP || p->edge == EDGE_BOTTOM)
        ? ORIENT_HORIZ : ORIENT_VERT;
    if (p->orientation == ORIENT_HORIZ) {
        p->my_box_new = gtk_hbox_new;
    } else {
        p->my_box_new = gtk_vbox_new;
    }
    if (p->width < 0)
        p->width = 100;
    if (p->widthtype == WIDTH_PERCENT && p->width > 100)
        p->width = 100;
    p->heighttype = HEIGHT_PIXEL;
    if (p->heighttype == HEIGHT_PIXEL) {
        if (p->height < PANEL_HEIGHT_MIN)
            p->height = PANEL_HEIGHT_MIN;
        else if (p->height > PANEL_HEIGHT_MAX)
            p->height = PANEL_HEIGHT_MAX;
    }
    panel_start_gui(p);
    RET(1);
}

int
panel_start(panel *p)
{
    /* parse global section */
    ENTER;

    if (!panel_parse_global(p))
        RET(0);

    if (!tray_constructor(p))
        RET(0);

    gtk_widget_show_all(p->topgwin);
    RET(1);
}

void panel_stop(panel *p)
{
    ENTER;

    tray_destructor(p);
    XSelectInput (gdk_helper_display(), GDK_ROOT_WINDOW(), NoEventMask);
    gdk_window_remove_filter(gdk_get_default_root_window (), (GdkFilterFunc)panel_wm_events, p);
    gtk_widget_destroy(p->topgwin);
    RET();
}

void
usage()
{
    ENTER;
    printf("trayer %s - lightweight GTK2+ systray for UNIX desktops\n", version);
    printf("Command line options:\n");
    printf(" -h  -- print this help and exit:\n");
    printf(" -v  -- print version and exit:\n");
    printf(" --edge       <left|right|top|bottom|none> (default:bottom) \n");
    printf(" --align      <left|right|center>          (default:center)\n");
    printf(" --margin     <number>\n");
    printf(" --widthtype  <request|pixel|percent>      (default:percent)\n");
    printf(" --width      <number>                     (default:100)\n");
    printf(" --heighttype <request|pixel>              (default:pixel)\n");
    printf(" --height     <number>                     (default:26)\n");
    printf(" --SetDockType     <true|false>            (default:true)\n");
    printf(" --SetPartialStrut <true|false>            (default:false)\n");
    printf(" --transparent     <true|false>            (default:false)\n");
    printf(" --alpha      <number>                     (default:127)\n");
    printf(" --tint       <int>\n");
    printf(" --distance   <number>\n");
    printf(" --distancefrom <number>\n");
    printf(" --expand     <false|true>\n");
    printf(" --padding    <number>\n");
    printf(" --monitor    <number>                     (default:0)\n");
}
    
void
handle_error(Display * d, XErrorEvent * ev)
{
    char buf[256];

    ENTER;
    XGetErrorText(gdk_helper_display(), ev->error_code, buf, 256);
    ERR( "trayer : X error: %s\n", buf);
    RET();
}

int
main(int argc, char *argv[], char *env[])
{
    int i;

    ENTER;
    setlocale(LC_CTYPE, ""); 
    gtk_init(&argc, &argv);
    XSetLocaleModifiers(""); 
    XSetErrorHandler((XErrorHandler) handle_error);
    // resolve xatoms 
    resolve_atoms();

    p = g_new0(panel, 1);
    memset(p, 0, sizeof(panel));
    p->allign = ALLIGN_CENTER;
    p->edge = EDGE_BOTTOM;
    p->widthtype = WIDTH_PERCENT;
    p->width = 100;
    p->heighttype = HEIGHT_PIXEL;
    p->height = PANEL_HEIGHT_DEFAULT;
    p->setdocktype = 1;
    p->setstrut = 0;
    p->transparent = 0;
    p->alpha = 127;
    p->tintcolor = 0xFFFFFFFF;
    p->xtopbg = None;
    p->monitor = 0;

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            usage();
            exit(0);
        } else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            printf("trayer %s\n", version);
            exit(0);
        } else if (!strcmp(argv[i], "--edge")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing edge parameter value\n");
                usage();
                exit(1);
            } else {
                p->edge = str2num(edge_pair, argv[i], EDGE_NONE);
            }
        } else if (!strcmp(argv[i], "--align")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing align parameter value\n");
                usage();
                exit(1);
            } else {
                p->allign = str2num(allign_pair, argv[i], ALLIGN_NONE);
            }
        } else if (!strcmp(argv[i], "--margin")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing margin parameter value\n");
                usage();
                exit(1);
            } else {
                p->margin = atoi(argv[i]);
            }
        } else if (!strcmp(argv[i], "--widthtype")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing widthtype parameter value\n");
                usage();
                exit(1);
            } else {
                p->widthtype = str2num(width_pair, argv[i], WIDTH_NONE);
            }
        } else if (!strcmp(argv[i], "--width")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing width parameter value\n");
                usage();
                exit(1);
            } else {
                p->width = atoi(argv[i]);
            }
        } else if (!strcmp(argv[i], "--heighttype")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing heighttype parameter value\n");
                usage();
                exit(1);
            } else {
                p->heighttype = str2num(height_pair, argv[i], HEIGHT_NONE);
            }
        } else if (!strcmp(argv[i], "--height")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing height parameter value\n");
                usage();
                exit(1);
            } else {
                p->height = atoi(argv[i]);
            }
        } else if (!strcmp(argv[i], "--SetDockType")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing SetDockType parameter value\n");
                usage();
                exit(1);
            } else {
                p->setdocktype = str2num(bool_pair, argv[i], 0);
            }
        } else if (!strcmp(argv[i], "--SetPartialStrut")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing SetPartialStrut parameter value\n");
                usage();
                exit(1);
            } else {
                p->setstrut = str2num(bool_pair, argv[i], 0);
            }
        } else if (!strcmp(argv[i], "--transparent")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing transparent parameter value\n");
                usage();
                exit(1);
            } else {
                p->transparent = str2num(bool_pair, argv[i], 1);
            }
        } else if (!strcmp(argv[i], "--alpha")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing alpha parameter value\n");
                usage();
                exit(1);
            } else {
                p->alpha = atoi(argv[i]);
            }
        } else if (!strcmp(argv[i], "--tint")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing tint parameter value\n");
                usage();
                exit(1);
            } else {
                p->tintcolor = strtoul(argv[i], NULL, 0);
            }
        } else if (!strcmp(argv[i], "--distance")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing distance parameter value\n");
                usage();
                exit(1);
            } else {
                distance = atoi(argv[i]);
            }
        } else if (!strcmp(argv[i], "--distancefrom")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing distancefrom parameter value\n");
                usage();
                exit(1);
            } else {
                distancefrom = str2num(distancefrom_pair, argv[i], DISTANCEFROM_NONE);
            }
        } else if (!strcmp(argv[i], "--expand")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing expand parameter value\n");
                usage();
                exit(1);
            } else {
                expand = str2num(bool_pair, argv[i], 1);
            }
        } else if (!strcmp(argv[i], "--padding")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing padding parameter value\n");
                usage();
                exit(1);
            } else {
                padding = atoi(argv[i]);
            }
        } else if (!strcmp(argv[i], "--monitor")) {
            i++;
            if (i == argc) {
                ERR( "trayer: missing monitor parameter value\n");
                usage();
                exit(1);
            } else {
                p->monitor = atoi(argv[i]);
            }
        } else {
            printf("trayer: unknown option - %s\n", argv[i]);
            usage();
            exit(1);
        }
    }
    g_return_val_if_fail (p != NULL, 1);
    if (!panel_start(p)) {
        ERR( "trayer: can't start panel\n");
        exit(1);
    }
    gtk_main();
    panel_stop(p);
    g_free(p);

    exit(0);
}

