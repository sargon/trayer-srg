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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
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

#ifndef __GTK_BAR_H__
#define __GTK_BAR_H__


#include <gdk/gdk.h>
#include <gtk/gtkbox.h>


#ifdef __cplusplus
//extern "C" {
#endif /* __cplusplus */


#define GTK_TYPE_BAR            (gtk_bar_get_type ())
#define GTK_BAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_BAR, GtkBar))
#define GTK_BAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_BAR, GtkBarClass))
#define GTK_IS_BAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_BAR))
#define GTK_IS_BAR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_BAR))
#define GTK_BAR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_BAR, GtkBarClass))


typedef struct _GtkBar       GtkBar;
typedef struct _GtkBarClass  GtkBarClass;
typedef enum { GTK_BAR_VERTICAL, GTK_BAR_HORIZ } GtkBarOrientation;

struct _GtkBar
{
    GtkBox box;
    gint max_child_size;
    GtkBarOrientation orient;
};

struct _GtkBarClass
{
    GtkBoxClass parent_class;
};


GType	   gtk_bar_get_type (void) G_GNUC_CONST;
GtkWidget* gtk_bar_new      (GtkBarOrientation orient, gint spacing);
void       gtk_bar_set_max_child_size(GtkBar *bar, gint size);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_BAR_H__ */
