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
#include "xkcd-api.h"

struct _XkcdsWindow
{
    GtkApplicationWindow parent;
};

typedef struct
{
    GtkWidget *imageview;
    GtkWidget *download;
    GtkWidget *random;
    GtkWidget *next;
    GtkWidget *prev;
    GtkWidget *stack;
    GtkWidget *image_event;
    GtkWidget *spinner;
    GList *cache;
    GdkPixbuf *Image;
} XkcdsWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (XkcdsWindow, xkcds_window, GTK_TYPE_APPLICATION_WINDOW)

enum {
    PROP_0,
    N_PROPS
};

static GParamSpec *properties [N_PROPS];

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

    G_OBJECT_CLASS (xkcds_window_parent_class)->finalize (object);
}

static void
xkcds_window_get_property (GObject    *object,
                           guint       prop_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
    XkcdsWindow *self = XKCDS_WINDOW (object);

    switch (prop_id)
      {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      }
}

static void
xkcds_window_set_property (GObject      *object,
                           guint         prop_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
    XkcdsWindow *self = XKCDS_WINDOW (object);

    switch (prop_id)
      {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      }
}

static void
xkcds_window_class_init (XkcdsWindowClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (object_class), "/xyz/vyasgiridhar/xkcds/xkcds-window.ui");

    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (object_class), XkcdsWindow, imageview);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (object_class), XkcdsWindow, stack);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (object_class), XkcdsWindow, image_event);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (object_class), XkcdsWindow, spinner);
    //gtk_widget_bind_from_template_child_private (GTK_WIDGET_CLASs (object_class), XkcdsWindow, prev);

    object_class->finalize = xkcds_window_finalize;
    object_class->get_property = xkcds_window_get_property;
    object_class->set_property = xkcds_window_set_property;
}

static void
xkcds_window_init (XkcdsWindow *self)
{
    XkcdsWindowPrivate *priv = xkcds_window_get_instance_private (self);
    gtk_widget_init_template (GTK_WIDGET (self));

    gtk_spinner_start (GTK_SPINNER (priv->spinner));
    gtk_stack_set_visible_child (GTK_STACK (priv->stack), priv->spinner);
    
    XkcdApi *api_ref= xkcd_api_new();
    //gtk_image_set_from_resource (GTK_IMAGE (priv->imageview), "/xyz/vyasgiridhar/xkcds/random.png");
}
