
#include "plugin.h"

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk-pixbuf-xlib/gdk-pixbuf-xlib.h>
#include <gdk/gdk.h>
#include <string.h>


//#define DEBUG
#include "dbg.h"
#include "misc.h"

static GList *pcl = NULL;


/* counter for static (built-in) plugins must be greater then zero
 * so fbpanel will not try to unload them */
#define STATIC_PLUGIN_CLASS(spc) \
do { \
extern plugin_class spc;\
pcl = g_list_append(pcl, &spc);\
spc.count++;\
spc.dynamic = 0;\
} while (0)

 
static void
init_plugin_class_list()
{
    ENTER;
      
#ifdef STATIC_TRAY
    STATIC_PLUGIN_CLASS(tray_plugin_class);
#endif

    RET();
}





plugin *
plugin_load(char *type)
{
    GList *tmp;
    plugin_class *pc = NULL;
    plugin *plug = NULL;
    static GString *str = NULL;
    
    ENTER;
    if (!pcl)
        init_plugin_class_list();

    for (tmp = pcl; tmp; tmp = g_list_next(tmp)) {
        pc = (plugin_class *) tmp->data;
        if (!g_ascii_strcasecmp(type, pc->type)) {
            break;
        }
    }
    if (!tmp && g_module_supported()) {
        GModule *m;
        
        if (!str)
            str = g_string_sized_new(PATH_MAX);
        g_string_printf(str, "%s/share/trayer/plugins/%s.so", PREFIX, type);
        m = g_module_open(str->str, G_MODULE_BIND_LOCAL | G_MODULE_BIND_LAZY);
        if (!m)
            RET(NULL);
        
        g_string_printf(str, "%s_plugin_class", type);
        if (!g_module_symbol(m, str->str, (gpointer *)&pc))
            RET(NULL);
        pc->gmodule = m;
        pc->dynamic = 1;
    }

    /* nothing was found */
    if (!pc)
        RET(NULL);
    
    plug = g_new0(plugin, 1);
    g_return_val_if_fail (plug != NULL, NULL);
    plug->class = pc;
    pc->count++;
    RET(plug);
}


void plugin_put(plugin *this)
{
    ENTER;
    if (this->class->dynamic)
        g_module_close(this->class->gmodule);
    //if (this->class->count == 0)
    //ERR( "unloading plugin %s\n", this->class->type);
    g_free(this);
    RET();
}


#if 0
static void
plugin_style_set(GtkWidget *widget, GtkStyle *s, plugin *p)
{
    ENTER;
    /*
    if (p->double_buffered) 
        gtk_widget_set_double_buffered (widget, FALSE);
    if (GTK_WIDGET_REALIZED(widget)) {
        gdk_window_clear(widget->window);
        
    if (p->double_buffered)
        gtk_widget_set_double_buffered (widget, TRUE);
        gtk_widget_queue_draw(widget);
    */
    RET();
    
}

static gint
plugin_size_alloc(GtkWidget *widget, GtkAllocation *a, plugin *p)
{
    static GtkAllocation a2;
    
    ENTER;
    if (memcmp(&a2, a, sizeof(GtkAllocation))) {
        GdkRectangle rect;

        gdk_window_clear(widget->window);

        rect.x = rect.y = 0;
        rect.width = widget->allocation.width;
        rect.height = widget->allocation.height;
        gdk_window_invalidate_rect(widget->window, &rect, TRUE);
        gtk_style_apply_default_background (widget->style, widget->window, TRUE, widget->state,
              &rect, 0, 0, rect.width, rect.height);
        a2 = *a;
        DBG("size changed for %s\n", p->class->type);
     
        /*
        gtk_widget_set_double_buffered (widget, FALSE);
        gdk_window_clear(widget->window);
        gtk_widget_queue_draw(widget);
        //gtk_widget_set_double_buffered (widget, TRUE);
        p->double_buffered = 0;
        */
    }
    RET(TRUE);
}

#endif


int
plugin_start(plugin *this)
{
    //panel *p = this->panel;
    ENTER;
    this->pwid = gtk_event_box_new();
    if (this->panel->transparent) {
        /*
        g_signal_connect (G_OBJECT (this->pwid), "style-set",
              (GCallback) plugin_style_set, this);
        g_signal_connect (G_OBJECT (this->pwid), "size-allocate",
              (GCallback) plugin_size_alloc, this);
        this->double_buffered = 0;
        //gtk_widget_set_double_buffered (this->pwid, FALSE);
        */
    }
    gtk_box_pack_start(GTK_BOX(this->panel->box), this->pwid, this->expand, TRUE,
          this->padding);
    
    if (!this->class->constructor(this)) {
        gtk_widget_destroy(this->pwid);
        RET(0);
    }
    gtk_widget_show(this->pwid);
   
    //g_timeout_add(3000, (GSourceFunc) clear, (gpointer)this->pwid);
    //gdk_window_clear(this->pwid->window); 
    RET(1);
}


void plugin_stop(plugin *this)
{
    ENTER;
    this->class->destructor(this);
    this->panel->plug_num--;
    gtk_widget_destroy(this->pwid);
    RET();
}

