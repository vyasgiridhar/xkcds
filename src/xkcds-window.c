/* xkcds-window.c
 *
 * Copyright (C) 2017 Vyas Giridharan <vyasgiridhar27@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "xkcds-window.h"
#include "xkcds-image.h"

struct _XkcdsWindow
{
    GtkApplicationWindow parent;
};

typedef struct
{
    XkcdsImage *xkcds_image;
    GList *cache;
    GdkPixbuf *Image;
} XkcdsWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (XkcdsWindow, xkcds_window, GTK_TYPE_APPLICATION_WINDOW)

XkcdsWindow *
xkcds_window_new (XkcdsApplication *app)
{
    return g_object_new (XKCDS_TYPE_WINDOW, "application", app, NULL);
}

static void
xkcds_window_finalize (GObject *object)
{
    XkcdsWindow *self = (XkcdsWindow *)object;
    XkcdsWindowPrivate *priv = xkcds_window_get_instance_private (self);

    g_object_unref (priv->xkcds_image);

    G_OBJECT_CLASS (xkcds_window_parent_class)->finalize (object);
}

static void
xkcds_window_class_init (XkcdsWindowClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_ensure (XKCDS_TYPE_IMAGE);

    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (object_class), "/xyz/vyasgiridhar/xkcds/xkcds-window.ui");

    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (object_class), XkcdsWindow, xkcds_image);

    object_class->finalize = xkcds_window_finalize;
}

static void
xkcds_window_init (XkcdsWindow *self)
{
    XkcdsWindowPrivate *priv = xkcds_window_get_instance_private (self);

    gtk_widget_init_template (GTK_WIDGET (self));

    gtk_window_set_title (GTK_WINDOW (self), "Xkcds");
    gtk_window_set_resizable (GTK_WINDOW (self), FALSE);

    gtk_widget_show_all (GTK_WIDGET (self));

    xkcds_image_randomize (priv->xkcds_image);
}
