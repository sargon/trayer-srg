/* Fixed tooltip routine */

/* 
 * Copyright (C) 2001 Havoc Pennington, 2002 Red Hat Inc.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef FIXED_TIP_H
#define FIXED_TIP_H

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

/* root_x, root_y are where the speech balloon should be
 * "pointing" and the strut is the panel edge we should be
 * alongside.
 */
void fixed_tip_show (int screen_number,
                     int root_x, int root_y,
                     gboolean strut_is_vertical,
                     int strut,
                     const char *markup_text);
void fixed_tip_hide (void);


#endif
