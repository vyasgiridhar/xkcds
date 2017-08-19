/* xkcd-imageview.c
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

#include "xkcd-imageview.h"

struct _XkcdImageview
{
    GtkBin parent_instance;
};

typedef struct {
    GtkEventBox image_event;
    GtkImage    image;
} XkcdImageviewPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (XkcdImageview, xkcd_imageview, GTK_TYPE_BIN)

enum {
    PROP_0,
    N_PROPS
};

static GParamSpec *properties [N_PROPS];

XkcdImageview *
xkcd_imageview_new (void)
{
    return g_object_new (XKCD_TYPE_IMAGEVIEW, NULL);
}

static void
xkcd_imageview_finalize (GObject *object)
{
    XkcdImageview *self = (XkcdImageview *)object;

    G_OBJECT_CLASS (xkcd_imageview_parent_class)->finalize (object);
}

static void
xkcd_imageview_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
    XkcdImageview *self = XKCD_IMAGEVIEW (object);

    switch (prop_id)
      {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      }
}

static void
xkcd_imageview_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
    XkcdImageview *self = XKCD_IMAGEVIEW (object);

    switch (prop_id)
      {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      }
}

static void
xkcd_imageview_class_init (XkcdImageviewClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass), "/xyz/vyasgiridhar/xkcd/xkcd-imageview.ui");

    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), XkcdImageview, image_event);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), XkcdImageview, image);

    object_class->finalize = xkcd_imageview_finalize;
    object_class->get_property = xkcd_imageview_get_property;
    object_class->set_property = xkcd_imageview_set_property;
}

static void
xkcd_imageview_init (XkcdImageview *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));
}
