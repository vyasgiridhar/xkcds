/* xkcds-application.c
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

#include "xkcds-application.h"
#include "xkcds-window.h"

struct _XkcdsApplication
{
    GtkApplication parent;
};

typedef struct
{
    XkcdsWindow *window;
} XkcdsApplicationPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (XkcdsApplication, xkcds_application, GTK_TYPE_APPLICATION)

enum {
    PROP_0,
    N_PROPS
};

static GParamSpec *properties [N_PROPS];

XkcdsApplication *
xkcds_application_new (void)
{
    return g_object_new (XKCDS_TYPE_APPLICATION,
                         "application-id", "xyz.vyasgiridhar.xkcd",
                         "flags", G_APPLICATION_HANDLES_OPEN,
                         NULL);
}

static void
xkcds_application_activate (GApplication *app)
{
    XkcdsWindow *window = xkcds_window_new (XKCDS_APPLICATION (app));

    gtk_window_present (GTK_WINDOW (window));
}

static void
xkcds_application_open (GApplication *app,
                        GFile       **files,
                        gint          n_files,
                        const gchar  *hint)
{
    XkcdsWindow *window = xkcds_window_new (XKCDS_APPLICATION (app));

    gtk_window_present (GTK_WINDOW (window));
}

static void
xkcds_application_finalize (GObject *object)
{
    XkcdsApplication *self = (XkcdsApplication *)object;
    XkcdsApplicationPrivate *priv = xkcds_application_get_instance_private (self);

    G_OBJECT_CLASS (xkcds_application_parent_class)->finalize (object);
}

static void
xkcds_application_get_property (GObject    *object,
                                guint       prop_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
    XkcdsApplication *self = XKCDS_APPLICATION (object);

    switch (prop_id)
      {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      }
}

static void
xkcds_application_set_property (GObject      *object,
                                guint         prop_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
    XkcdsApplication *self = XKCDS_APPLICATION (object);

    switch (prop_id)
      {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      }
}

static void
xkcds_application_class_init (XkcdsApplicationClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = xkcds_application_finalize;
    G_APPLICATION_CLASS (object_class)->open = xkcds_application_open;
    G_APPLICATION_CLASS (object_class)->activate = xkcds_application_activate;
//  G_APPLICATION_CLASS (object_class)->open = xkcds_application_startup;
    object_class->get_property = xkcds_application_get_property;
    object_class->set_property = xkcds_application_set_property;
}

static void
xkcds_application_init (XkcdsApplication *self)
{

}