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

#include <errno.h>

#include "xkcd-api.h"

struct _XkcdApi
{
	GObject      parent_instance;

	SoupSession *sesh;
    JsonParser  *parser;
	Xkcd        *xkcd;
};

G_DEFINE_TYPE (XkcdApi, xkcd_api, G_TYPE_OBJECT)

#define XKCD_API_ERROR 1

enum {
    XKCD_NETWORK_ERROR,
    XKCD_PIXBUF_ERROR,
};

enum {
    PREV = 1,
    NEXT,
    RAND,
};

XkcdApi *
xkcd_api_new (void)
{
	return g_object_new (XKCD_TYPE_API, NULL);
}

static void
xkcd_api_finalize (GObject *object)
{
	G_OBJECT_CLASS (xkcd_api_parent_class)->finalize (object);
}

static void
xkcd_api_dispose (GObject *gobject)
{
    XkcdApi *self = XKCD_API (gobject);

    if (self->parser)
    {
        g_clear_object (&self->parser);
    }

    if (self->sesh)
    {
        soup_session_abort (self->sesh);
        g_clear_object (&self->sesh);
    }

    if (self->xkcd)
    {
        g_clear_object (&self->xkcd);
    }

    G_OBJECT_CLASS (xkcd_api_parent_class)->dispose (gobject);
}

static void
xkcd_api_loader_thread (GTask *task,
                        gpointer source_object,
                        gpointer task_data,
                        GCancellable *cancellable)
{
    XkcdApi *self = source_object;
    GdkPixbufLoader *loader = NULL;
    SoupMessage *msg;
    GError *error = NULL;
    gchar *cache_dir;
    gchar *cached_xkcd;
    gchar *cmovement = NULL;
    gchar *url;
    int *movement = task_data;
    int num;
    int status;

    num = xkcd_object_get_number (self->xkcd);

    if (num == 0)
    {
        xkcd_object_set_number (self->xkcd, g_random_int () % 1000);
    }
    else
    {
        switch (*movement)
        {
            case PREV:
                xkcd_object_set_number (self->xkcd, --num);
                break;
            case NEXT:
                xkcd_object_set_number (self->xkcd, ++num);
                break;
            case RAND:
                xkcd_object_set_number (self->xkcd, g_random_int () % 1000);
                break;
            default:
                break;
        }
    }

    num = xkcd_object_get_number (self->xkcd);

    cached_xkcd = g_build_filename (g_get_user_cache_dir (),
                                    "xkcds",
                                    "images",
                                    "1",
                                    NULL);

    url = g_strdup_printf (XKCD_URL, num);

    msg = soup_message_new (SOUP_METHOD_GET, url);

    status = soup_session_send_message (self->sesh, msg);

    if (!SOUP_STATUS_IS_SUCCESSFUL (status))
    {
        error = g_error_new (XKCD_API_ERROR,
                             XKCD_NETWORK_ERROR,
                             "%s", msg->response_body->data);

        g_object_unref (msg);
        goto out;
    }

    g_clear_error (&error);
    g_clear_object (&self->xkcd);

    self->parser = json_parser_new ();
    json_parser_load_from_data (self->parser, msg->response_body->data, -1, NULL);

    JsonNode *root = json_parser_get_root(self->parser);
    self->xkcd = XKCD_OBJECT (json_gobject_deserialize (XKCD_TYPE_OBJECT, root));

    g_object_unref (msg);

    if (g_file_test (cached_xkcd, G_FILE_TEST_EXISTS))
    {
        gdk_pixbuf_new_from_file(cached_xkcd, &error);
        if (error)
        {
            g_warning ("Unable to load from cache");
            g_free (cached_xkcd);
            goto soup;
        }
    }
    else
    {
    soup:
        g_clear_error (&error);

        msg = soup_message_new (SOUP_METHOD_GET, xkcd_object_get_img_link (self->xkcd));
        status = soup_session_send_message (self->sesh, msg);

        if (!SOUP_STATUS_IS_SUCCESSFUL (status))
        {
            error = g_error_new (XKCD_API_ERROR,
                                 XKCD_NETWORK_ERROR,
                                 "%s",msg->reason_phrase);
            g_object_unref (msg);
            goto out;
        }

        loader = gdk_pixbuf_loader_new ();

        if (!gdk_pixbuf_loader_write (loader,
                                      (const guchar *) msg->response_body->data,
                                      msg->response_body->length,
                                      &error))
        {
            g_critical ("Unable to load pixbuf : %s", error->message);
            goto out;
        }

        g_object_unref (G_OBJECT (msg));
        g_clear_error (&error);

        gdk_pixbuf_loader_close (loader, &error);
        if (error)
        {
            g_warning ("Unable to close the pixbuf loader: %s", error->message);
            g_object_unref (loader);
            goto out;
        }
        else
        {
            cache_dir = g_build_filename (g_get_user_cache_dir (),
                                          "xkcds",
                                          "images",
                                          NULL);

            if (g_mkdir_with_parents (cache_dir, 0700) == -1)
            {
                if (errno != EEXIST)
                {
                    error->message = "Unable to create the profile image cache";
                    g_object_unref (loader);
                    goto out;
                }
            }

            cmovement = g_strdup_printf ("%d", num);

            cached_xkcd = g_build_filename (cache_dir,
                                            cmovement,
                                            NULL);

            g_file_set_contents (cached_xkcd,
                                 msg->response_body->data,
                                 msg->response_body->length,
                                 NULL);
            g_free (cache_dir);
            g_free (cached_xkcd);
        }

        GdkPixbuf *temp = gdk_pixbuf_loader_get_pixbuf (loader);
        xkcd_object_set_pixbuf (self->xkcd, temp);
        g_clear_object (&temp);
    }

    g_clear_error (&error);
    g_free (url);
    g_free (movement);
    g_free (cmovement);
    gdk_pixbuf_loader_close (loader, &error);
    g_clear_object (&loader);

    g_task_return_pointer (task, self->xkcd, g_object_unref);
    return;
    out:
    g_task_return_error (task, error);
    return;
}

void
xkcd_api_load_async (XkcdApi            *self,
                     int                *movement,
                     GCancellable       *cancellable,
                     GAsyncReadyCallback callback,
                     gpointer            data)
{
    GTask *task;

    task = g_task_new (self, cancellable, callback, data);

    g_task_set_task_data (task, movement, NULL);

    g_task_run_in_thread (task, xkcd_api_loader_thread);

    g_object_unref (task);
}

Xkcd*
xkcd_api_load_finish (XkcdApi      *self,
                      GAsyncResult *result,
                      GError      **error)
{
    g_return_val_if_fail (g_task_is_valid (result, self), NULL);

    return g_task_propagate_pointer (G_TASK (result), error);
}

static void
xkcd_api_class_init (XkcdApiClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = xkcd_api_finalize;
    object_class->dispose = xkcd_api_dispose;
}

static void
xkcd_api_init (XkcdApi *self)
{
    self->xkcd = xkcd_object_new ();
    self->sesh = soup_session_new ();
}
