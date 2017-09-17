/* xkcd.c
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

#include "xkcd.h"

typedef struct
{
    gchar       *month;
    int          num;
    gchar       *link;
    gchar       *year;
    gchar       *news;
    gchar       *safe_title;
    gchar       *transcript;
    gchar       *alt;
    gchar       *img;
    gchar       *title;
    gchar       *day;

    GQuark       quark;
    SoupSession *sesh;
    GdkPixbuf   *cache;
} XkcdPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (Xkcd, xkcd_object, G_TYPE_OBJECT)

#define XKCD_API_ERROR 1

enum {
	LOADED,
	N_SIGNALS
};

enum {
    PREV,
    NEXT,
    RAND
};

enum {
    XKCD_NETWORK_ERROR,
    XKCD_PIXBUF_ERROR
};

enum {
    PROP_MONTH = 1,
    PROP_NUM,
    PROP_LINK,
    PROP_YEAR,
    PROP_NEWS,
    PROP_SAFE_TITLE,
    PROP_TRANSCRIPT,
    PROP_ALT,
    PROP_IMG,
    PROP_TITLE,
    PROP_DAY,
    N_PROPS
};

static guint signals [N_SIGNALS] = {0, };

static GParamSpec *properties [N_PROPS]  = {NULL,};

Xkcd *
xkcd_object_new (void)
{
    return g_object_new (XKCD_TYPE_OBJECT, NULL);
}

static void
xkcd_object_finalize (GObject *object)
{
    Xkcd *self = (Xkcd *)object;
    XkcdPrivate *priv = xkcd_object_get_instance_private (self);

    g_free (priv->month);
    g_free (priv->link);
    g_free (priv->year);
    g_free (priv->news);
    g_free (priv->safe_title);
    g_free (priv->transcript);
    g_free (priv->alt);
    g_free (priv->img);
    g_free (priv->title);
    g_free (priv->day);

    G_OBJECT_CLASS (xkcd_object_parent_class)->finalize (object);
}

static void
xkcd_object_get_property (GObject    *object,
                   guint       prop_id,
                   GValue     *value,
                   GParamSpec *pspec)
{
    Xkcd *self = XKCD_OBJECT (object);
    XkcdPrivate *priv = xkcd_object_get_instance_private (self);

    switch (prop_id)
      {
          case PROP_MONTH:
              g_value_set_string (value, priv->month);
              break;
          case PROP_NUM:
              g_value_set_int (value, priv->num);
              break;
          case PROP_LINK:
              g_value_set_string (value, priv->link);
              break;
          case PROP_YEAR:
              g_value_set_string (value, priv->year);
              break;
          case PROP_NEWS:
              g_value_set_string (value, priv->news);
              break;
          case PROP_SAFE_TITLE:
              g_value_set_string (value, priv->safe_title);
              break;
          case PROP_TRANSCRIPT:
              g_value_set_string (value, priv->transcript);
              break;
          case PROP_ALT:
              g_value_set_string (value, priv->alt);
              break;
          case PROP_IMG:
              g_value_set_string (value, priv->img);
              break;
          case PROP_TITLE:
              g_value_set_string (value, priv->title);
              break;
          case PROP_DAY:
              g_value_set_string (value, priv->day);
              break;
          default:
              G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      }
}

static void
xkcd_object_set_property (GObject      *object,
                   guint         prop_id,
                   const GValue *value,
                   GParamSpec   *pspec)
{
    Xkcd *self = XKCD_OBJECT (object);
    XkcdPrivate *priv = xkcd_object_get_instance_private (self);

    switch (prop_id)
      {
          case PROP_MONTH:
              priv->month = g_strdup (g_value_get_string (value));
              break;
          case PROP_NUM:
              priv->num = g_value_get_int (value);
              break;
          case PROP_LINK:
              priv->link = g_strdup (g_value_get_string (value));
              break;
          case PROP_YEAR:
              priv->year = g_strdup (g_value_get_string (value));
              break;
          case PROP_NEWS:
              priv->news = g_strdup (g_value_get_string (value));
              break;
          case PROP_SAFE_TITLE:
              priv->safe_title = g_strdup (g_value_get_string (value));
              break;
          case PROP_TRANSCRIPT:
              priv->transcript = g_strdup (g_value_get_string (value));
              break;
          case PROP_ALT:
              priv->alt = g_strdup (g_value_get_string (value));
              break;
          case PROP_IMG:
              priv->img = g_strdup (g_value_get_string (value));
              break;
          case PROP_TITLE:
              priv->title = g_strdup (g_value_get_string (value));
              break;
          case PROP_DAY:
              priv->day = g_strdup (g_value_get_string (value));
              break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      }
}

static void
xkcd_object_class_init (XkcdClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = xkcd_object_finalize;
    object_class->get_property = xkcd_object_get_property;
    object_class->set_property = xkcd_object_set_property;

    properties [PROP_MONTH] =
        g_param_spec_string ("month",
                             "Month",
                             "Month of the XKCD post",
                             NULL,
                             G_PARAM_READWRITE);
    properties [PROP_NUM] =
        g_param_spec_int  ("num",
                           "Num",
                           "Number of the XKCD",
                           0,
                           10000000,
                           0,
                           G_PARAM_READWRITE);
    properties [PROP_LINK] =
        g_param_spec_string ("link",
                             "Link",
                             "Link to the XKCD",
                             NULL,
                             G_PARAM_READWRITE);
    properties [PROP_YEAR] =
        g_param_spec_string ("year",
                             "Year",
                             "Year of the posting",
                             NULL,
                             G_PARAM_READWRITE);
    properties [PROP_NEWS] =
        g_param_spec_string ("news",
                             "News",
                             "News about the XKCD",
                             NULL,
                             G_PARAM_READWRITE);
    properties [PROP_SAFE_TITLE] =
        g_param_spec_string ("safe_title",
                             "SafeTitle",
                             "Safe Title for the XKCD",
                             NULL,
                             G_PARAM_READWRITE);
    properties [PROP_TRANSCRIPT] =
        g_param_spec_string ("transcript",
                             "Transcript",
                             "Transcript for the XKCD",
                             NULL,
                             G_PARAM_READWRITE);
    properties [PROP_ALT] =
        g_param_spec_string ("alt",
                             "Alt",
                             "Alt text for the image",
                             NULL,
                             G_PARAM_READWRITE);
    properties [PROP_IMG] =
        g_param_spec_string ("img",
                             "Img",
                             "Img link to the XKCD",
                             NULL,
                             G_PARAM_READWRITE);
    properties [PROP_TITLE] =
        g_param_spec_string ("title",
                             "Title",
                             "Title for the XKCD",
                             NULL,
                             G_PARAM_READWRITE);
    properties [PROP_DAY] =
        g_param_spec_string ("day",
                             "Day",
                             "Day of the XKCD posting",
                             NULL,
                             G_PARAM_READWRITE);

	signals [LOADED] =
		g_signal_new ("loaded",
                      XKCD_TYPE_OBJECT,
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (XkcdClass, loaded),
                      NULL,
                      NULL,
                      NULL,
                      G_TYPE_NONE,
                      0);

    g_object_class_install_properties (object_class,
                                       N_PROPS,
                                       properties);
}

static void
xkcd_loader_thread (GTask *task,
                    gpointer source_object,
                    gpointer task_data,
                    GCancellable *cancellable)
{
    Xkcd *self = source_object;
    int *movement = task_data;

    XkcdPrivate *priv = xkcd_object_get_instance_private (self);
    Xkcd *new = xkcd_object_new ();

    char *url;
    GError *error;
    SoupMessage *msg;
    int status;

    error = g_slice_new0 (GError);

    if (priv->num == 0)
    {
        priv->num = g_random_int () % 1000;
    }
    else
    {
        switch (*movement)
        {
            case PREV:
                priv->num--;
                break;
            case NEXT:
                priv->num++;
                break;
            case RAND:
                priv->num = g_random_int () % 1000;
                break;
            default:
                break;
        }
    }

    url = g_strdup_printf (XKCD_URL, priv->num);

    msg = soup_message_new (SOUP_METHOD_GET, url);

    status = soup_session_send_message (priv->sesh, msg);

    if (!SOUP_STATUS_IS_SUCCESSFUL (status))
    {
        error->domain = XKCD_API_ERROR;
        error->code = XKCD_NETWORK_ERROR;
        error->message = g_strdup (msg->response_body->data);

        g_task_return_error (task, error);
    }

    JsonParser *parser = json_parser_new ();
    json_parser_load_from_data (parser, msg->response_body->data, -1, NULL);
    JsonNode *root = json_parser_get_root(parser);

    self = XKCD_OBJECT (json_gobject_deserialize (XKCD_TYPE_OBJECT, root));
    if (self)
    {
        g_task_return_pointer (task, new, g_object_unref);
    }
    else
        g_task_return_error (task, error);
}

void
xkcd_object_load_async (Xkcd               *self,
                        int                *movement,
                        GCancellable       *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer            data)
{
    GTask *task;

    task = g_task_new (self, cancellable, callback, data);

    g_task_set_task_data (task, movement, NULL);

    g_task_run_in_thread (task, xkcd_loader_thread);

    g_object_unref (task);
}

static void
xkcd_object_init (Xkcd *self)
{
    XkcdPrivate *priv = xkcd_object_get_instance_private (self);

    priv->num = 0;
    priv->sesh = soup_session_new ();
}
