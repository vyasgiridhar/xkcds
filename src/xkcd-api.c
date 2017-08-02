/* xkcd-api.c
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

#include "xkcd-api.h"

struct _XkcdApi
{
    GObject parent;
};

typedef struct
{
    SoupSession *sesh;
    int          random;
} XkcdApiPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (XkcdApi, xkcd_api, G_TYPE_OBJECT)

enum {
    PROP_0,
    N_PROPS
};

static GParamSpec *properties [N_PROPS];

XkcdApi *
xkcd_api_new (void)
{
    return g_object_new (XKCD_TYPE_API, NULL);
}

static void
xkcd_api_finalize (GObject *object)
{
    XkcdApi *self = (XkcdApi *)object;
    XkcdApiPrivate *priv = xkcd_api_get_instance_private (self);

    G_OBJECT_CLASS (xkcd_api_parent_class)->finalize (object);
}

static void
xkcd_api_get_property (GObject    *object,
                       guint       prop_id,
                       GValue     *value,
                       GParamSpec *pspec)
{
    XkcdApi *self = XKCD_API (object);

    switch (prop_id)
      {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      }
}

static void
xkcd_api_set_property (GObject      *object,
                       guint         prop_id,
                       const GValue *value,
                       GParamSpec   *pspec)
{
    XkcdApi *self = XKCD_API (object);

    switch (prop_id)
      {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      }
}

static void
xkcd_api_class_init (XkcdApiClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = xkcd_api_finalize;
    object_class->get_property = xkcd_api_get_property;
    object_class->set_property = xkcd_api_set_property;
}

Xkcd* xkcd_api_get_random (XkcdApi *self)
{
    Xkcd *xkcd = xkcd_new ();
    XkcdApiPrivate *priv = xkcd_api_get_instance_private (self);
    int random = g_random_int () % 1800; 
    
    gchar *url = "";
    g_snprintf (url, 100,"%s/%d/info.0.json", XKCD_API_URL, random);

    
    g_free (url);
    return xkcd;
}
static void
xkcd_api_init (XkcdApi *self)
{
    XkcdApiPrivate *priv = xkcd_api_get_instance_private (self);    

    priv->sesh = soup_session_new();

    SoupMessage *msg;

    msg = soup_message_new ("GET", "http://xkcd.com/info.0.json");
    guint status = soup_session_send_message (priv->sesh, msg);
    g_debug ("%d\n\n%s", status, msg->response_body->data);
//    SoupMessageBody *body = g_object_get (msg, "response-body");
}
