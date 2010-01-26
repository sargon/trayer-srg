/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#include "gtkbar.h"

#include "dbg.h"

#define MAX_CHILD_SIZE 150

static void gtk_bar_class_init    (GtkBarClass   *klass);
static void gtk_bar_init          (GtkBar        *box);
static void gtk_bar_size_request  (GtkWidget *widget, GtkRequisition *requisition);
static void gtk_bar_size_allocate (GtkWidget *widget, GtkAllocation  *allocation);
//static gint gtk_bar_expose        (GtkWidget *widget, GdkEventExpose *event);

static GtkBoxClass *parent_class = NULL;

GType
gtk_bar_get_type (void)
{
    static GType bar_type = 0;

    if (!bar_type)
    {
        static const GTypeInfo bar_info =
            {
                sizeof (GtkBarClass),
                NULL,		/* base_init */
                NULL,		/* base_finalize */
                (GClassInitFunc) gtk_bar_class_init,
                NULL,		/* class_finalize */
                NULL,		/* class_data */
                sizeof (GtkBar),
                0,		/* n_preallocs */
                (GInstanceInitFunc) gtk_bar_init,
            };

        bar_type = g_type_register_static (GTK_TYPE_BOX, "GtkBar",
              &bar_info, 0);
    }

    return bar_type;
}

static void
gtk_bar_class_init (GtkBarClass *class)
{
    GtkWidgetClass *widget_class;

    parent_class = g_type_class_peek_parent (class);
    widget_class = (GtkWidgetClass*) class;

    widget_class->size_request = gtk_bar_size_request;
    widget_class->size_allocate = gtk_bar_size_allocate;
    //widget_class->expose_event = gtk_bar_expose;

}

static void
gtk_bar_init (GtkBar *bar)
{
    bar->max_child_size = MAX_CHILD_SIZE;
    gtk_widget_set_redraw_on_allocate (GTK_WIDGET (bar), TRUE);
}

GtkWidget*
gtk_bar_new (GtkBarOrientation orient, gint spacing)
{
    GtkBar *bar;

    bar = g_object_new (GTK_TYPE_BAR, NULL);
    GTK_BOX (bar)->spacing = spacing;
    bar->orient = orient;
    return (GtkWidget *)bar;
}


static void
gtk_bar_size_request (GtkWidget *widget, GtkRequisition *requisition)
{
    GtkBox *box;
    GtkBar *bar;
    GtkBoxChild *child;
    GList *children;
    gint nvis_children;

    box = GTK_BOX (widget);
    bar = GTK_BAR (widget);
    requisition->width = 0;
    requisition->height = 0;
    nvis_children = 0;

    children = box->children;
    while (children) {
        child = children->data;
        children = children->next;

        if (GTK_WIDGET_VISIBLE (child->widget))	{
            GtkRequisition child_requisition;

            gtk_widget_size_request (child->widget, &child_requisition);
            if (bar->orient == GTK_BAR_HORIZ)
                requisition->height = MAX (requisition->height, child_requisition.height);
            else
                requisition->width = MAX (requisition->width, child_requisition.width);
            nvis_children++;
        }
    }

    if (nvis_children > 0) {
        if (bar->orient == GTK_BAR_HORIZ) {
            requisition->width = nvis_children * GTK_BAR(widget)->max_child_size;
            requisition->width += (nvis_children - 1) * box->spacing;
        } else {
            requisition->height = nvis_children * GTK_BAR(widget)->max_child_size;
            requisition->height += (nvis_children - 1) * box->spacing;
        }
    }

    requisition->width += GTK_CONTAINER (box)->border_width * 2;
    requisition->height += GTK_CONTAINER (box)->border_width * 2;
}

static void
gtk_bar_size_allocate (GtkWidget     *widget,
			GtkAllocation *allocation)
{
    GtkBox *box;
    GtkBar *bar;
    GtkBoxChild *child;
    GList *children;
    GtkAllocation child_allocation;
    gint nvis_children;
    gint bw;
    gint tmp;
    
    box = GTK_BOX (widget);
    bar = GTK_BAR (widget);
    widget->allocation = *allocation;

    bw = GTK_CONTAINER (box)->border_width;
    nvis_children = 0;
    children = box->children;
    while (children) {
        child = children->data;
        children = children->next;

        if (GTK_WIDGET_VISIBLE (child->widget))
            nvis_children += 1;
    }

    if (nvis_children == 0)
        return;
    
    child_allocation.x = allocation->x + bw;
    child_allocation.y = allocation->y + bw;

    if (bar->orient == GTK_BAR_HORIZ) {
        child_allocation.height = MAX (1, (gint) allocation->height - bw * 2);
        tmp = (allocation->width - bw * 2 - (nvis_children - 1) * box->spacing);
        child_allocation.width = MAX (1, MIN(tmp / nvis_children, GTK_BAR(widget)->max_child_size));
    } else {
        child_allocation.width = MAX (1, (gint) allocation->width - bw * 2);
        tmp = (allocation->height - bw * 2 - (nvis_children - 1) * box->spacing);
        child_allocation.height = MAX (1, MIN(tmp / nvis_children, GTK_BAR(widget)->max_child_size));
    }
    
    children = box->children;
    while (children) {
        child = children->data;
        children = children->next;

        if (GTK_WIDGET_VISIBLE (child->widget)) {
            gtk_widget_size_allocate (child->widget, &child_allocation);
            if (bar->orient == GTK_BAR_HORIZ)
                child_allocation.x += child_allocation.width + box->spacing;
            else
                child_allocation.y += child_allocation.height + box->spacing;
        }
    }
}


void
gtk_bar_set_max_child_size(GtkBar *bar, gint size)
{
    g_return_if_fail (GTK_IS_BAR (bar));
    
    if (size != bar->max_child_size) {
        bar->max_child_size = size;
        //g_object_notify (G_OBJECT (bar), "spacing");
        gtk_widget_queue_resize (GTK_WIDGET (bar));
    }
}


#if 0

static void
gtk_bar_paint (GtkWidget *widget, GdkRectangle *area)
{
    ENTER2;
    gdk_window_clear(widget->window);
    RET2();
    if (!GTK_WIDGET_APP_PAINTABLE (widget))
        gtk_paint_flat_box (widget->style, widget->window,
              widget->state, GTK_SHADOW_NONE,
              area, widget, NULL,
              0, 0, 20001, 20001);
    RET2();
}

static gint
gtk_bar_expose (GtkWidget *widget, GdkEventExpose *event)
{
    ENTER;
    RET(FALSE);
    /*
    DBG("pos=(%d,%d) dim=(%d,%d)\n", event->area.x, event->area.y, event->area.width, event->area.height);
    if (GTK_WIDGET_DRAWABLE (widget)) {
        DBG("is drawable\n");
        //if (!GTK_WIDGET_NO_WINDOW (widget))
        gtk_bar_paint (widget, &event->area);
      
        (* GTK_WIDGET_CLASS (parent_class)->expose_event) (widget, event);
        
    }
    RET(FALSE);
    */
}
/*
void
gtk_container_propagate_expose (GtkContainer   *container,
				GtkWidget      *child,
				GdkEventExpose *event)
{
  GdkEvent *child_event;

  ENTER;
  g_return_if_fail (GTK_IS_CONTAINER (container));
  g_return_if_fail (GTK_IS_WIDGET (child));
  g_return_if_fail (event != NULL);

  g_assert (child->parent == GTK_WIDGET (container));
  
  if (GTK_WIDGET_DRAWABLE (child) &&
      GTK_WIDGET_NO_WINDOW (child) &&
      (child->window == event->window))
    {
      child_event = gdk_event_new (GDK_EXPOSE);
      child_event->expose = *event;
      g_object_ref (child_event->expose.window);

      child_event->expose.region = gtk_widget_region_intersect (child, event->region);
      if (!gdk_region_empty (child_event->expose.region))
	{
	  gdk_region_get_clipbox (child_event->expose.region, &child_event->expose.area);
	  gtk_widget_send_expose (child, child_event);
	}
      gdk_event_free (child_event);
    }
}
*/
#endif
