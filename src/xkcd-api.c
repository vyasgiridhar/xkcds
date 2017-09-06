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
#include "xkcds-image.h"

struct _XkcdApi
{
    GObject parent_instance;
    SoupSession *sesh;
};

G_DEFINE_TYPE (XkcdApi, xkcd_api, G_TYPE_OBJECT)

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

static void
xkcd_api_handle_error (XkcdsImage *image, int status)
{
    g_warning ("status code: %d, Unable to fetch image", status);
}

void
xkcd_api_get_random (XkcdApi *self, XkcdsImage *image)
{
    int random;
    gchar* url;
    SoupMessage *msg;
    int status;
    Xkcd *xkcd;
    random = g_random_int () % 1800; 

    url = g_strdup_printf (XKCD_URL, random);
    msg = soup_message_new ("GET", url);

    status = soup_session_send_message (self->sesh, msg);
    if (status != 200) {
        xkcd_api_handle_error (image, status);
        return;
    }

    JsonParser *parser = json_parser_new ();
    json_parser_load_from_data (parser, msg->response_body->data, -1, NULL);

    JsonNode *root = json_parser_get_root(parser);
    xkcd = XKCD_OBJECT (json_gobject_deserialize (XKCD_TYPE_OBJECT, root));
/*
    gchar *alt, *img;
    g_object_get (xkcd, "alt", &alt, NULL);
    g_object_get (xkcd, "img", &img, NULL); 
    g_debug ("\n%s\n", alt);
    g_debug ("\n%s\n", img);
    g_free (alt);
    g_free (img);
*/
    g_free (url);
}
static void
xkcd_api_init (XkcdApi *self)
{
    self->sesh = soup_session_new ();
}
