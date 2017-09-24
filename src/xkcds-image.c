/* xkcds-image.c
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
#include "gtkimageview.h"

#define OVERLAY_REVEAL_ANIM_TIME (500U)

struct _XkcdsImage
{
    GtkStack      parent_instance;
    GtkOverlay   *overlay;
    GtkEventBox  *image_event;
    GtkImageView *imageview;
    GtkSpinner   *spinner;
    GtkWidget    *left;
    GtkWidget    *right;
    GtkWidget    *randomize;

    GSource      *overlay_timeout_source;
    GCancellable *cancel_action;

    guint        *current_movement;

    XkcdApi      *api;
};

G_DEFINE_TYPE (XkcdsImage, xkcds_image, GTK_TYPE_STACK)

enum {
    PROP_0,
    N_PROPS
};

static GParamSpec *properties [N_PROPS];

XkcdsImage *
xkcds_image_new (void)
{
    return g_object_new (XKCDS_TYPE_IMAGE, NULL);
}

static void
xkcds_image_finalize (GObject *object)
{
    XkcdsImage *self = (XkcdsImage *)object;

    g_object_unref (G_OBJECT (self->cancel_action));
    g_object_unref (self->api);

    self->cancel_action = NULL;
    G_OBJECT_CLASS (xkcds_image_parent_class)->finalize (object);
}

static void
xkcds_image_get_property (GObject    *object,
                          guint       prop_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
    XkcdsImage *self = XKCDS_IMAGE (object);

    switch (prop_id)
      {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      }
}

static void
xkcds_image_set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
    XkcdsImage *self = XKCDS_IMAGE (object);

    switch (prop_id)
      {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      }
}

static void
clear_overlay_timeout (XkcdsImage *self)
{
    if (self->overlay_timeout_source != NULL)
    {
        g_source_unref (self->overlay_timeout_source);
        g_source_destroy (self->overlay_timeout_source);
    }

    self->overlay_timeout_source = NULL;
}

static gboolean
overlay_timeout_callback (gpointer data)
{
    XkcdsImage *self = XKCDS_IMAGE (data);

    gtk_revealer_set_reveal_child (GTK_REVEALER (self->right), FALSE);
    gtk_revealer_set_reveal_child (GTK_REVEALER (self->left), FALSE);
    gtk_revealer_set_reveal_child (GTK_REVEALER (self->randomize), FALSE);

    clear_overlay_timeout (self);
    return FALSE;
}

static void
set_overlay_timeout (XkcdsImage *self)
{
    GSource *source;
    clear_overlay_timeout (self);

    source = g_timeout_source_new (1000);
    g_source_set_callback (source, overlay_timeout_callback, self, NULL);

    g_source_attach (source, NULL);

    self->overlay_timeout_source = source;
}

static gboolean
motion_notify_event (GtkWidget *widget, GdkEventMotion *event, gpointer user_data)
{
    XkcdsImage *self = XKCDS_IMAGE (user_data);
    gboolean reveal_child;

    reveal_child = gtk_revealer_get_reveal_child (GTK_REVEALER (self->right));

    if (!reveal_child)
    {
        gtk_revealer_set_reveal_child (GTK_REVEALER (self->right), TRUE);
        gtk_revealer_set_reveal_child (GTK_REVEALER (self->left), TRUE);
        gtk_revealer_set_reveal_child (GTK_REVEALER (self->randomize), TRUE);
    }

    set_overlay_timeout(self);
    return FALSE;
}

static gboolean
enter_overlay_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    XkcdsImage *self = XKCDS_IMAGE (user_data);

    clear_overlay_timeout (self);

    return FALSE;
}

void
xkcd_image_setter_callback (GObject      *source_object,
                            GAsyncResult *result,
                            gpointer      data)
{
    Xkcd *xkcd;
    XkcdApi *api = XKCD_API (source_object);
    XkcdsImage *image = data;
    GdkPixbuf *cache;
    GError *error;

    xkcd = xkcd_api_load_finish (api, result, &error);
    cache = xkcd_object_get_pixbuf_cache (xkcd);

    gtk_image_view_set_pixbuf (image->imageview,
                               cache,
                               1);
    gtk_stack_set_visible_child_name (GTK_STACK (image), "imageview");
}

void
xkcds_image_randomize (XkcdsImage *self)
{

    int *movement = 0;

    xkcd_api_load_async (self->api,
                         movement,
                         self->cancel_action,
                         xkcd_image_setter_callback,
                         self);

}

static void
xkcds_image_class_init (XkcdsImageClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_ensure (GTK_TYPE_IMAGE_VIEW);

    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass), "/xyz/vyasgiridhar/xkcds/xkcds-image.ui");

    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), XkcdsImage, imageview);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), XkcdsImage, spinner);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), XkcdsImage, overlay);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), XkcdsImage, image_event);

    object_class->finalize = xkcds_image_finalize;
    object_class->get_property = xkcds_image_get_property;
    object_class->set_property = xkcds_image_set_property;
}

static void
xkcds_image_init (XkcdsImage *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));

    self->cancel_action = g_cancellable_new ();
    self->api = xkcd_api_new ();

    /* left revealer */
    self->left = gtk_revealer_new ();
    gtk_revealer_set_transition_type (GTK_REVEALER (self->left),
                                      GTK_REVEALER_TRANSITION_TYPE_CROSSFADE);
    gtk_revealer_set_transition_duration (GTK_REVEALER (self->left),
                                          OVERLAY_REVEAL_ANIM_TIME);
    gtk_widget_set_halign (self->left, GTK_ALIGN_START);
    gtk_widget_set_valign (self->left, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start (self->left, 12);
    gtk_widget_set_margin_end (self->left, 12);
    gtk_overlay_add_overlay (self->overlay, self->left);

    /* right revealer */
    self->right = gtk_revealer_new ();
    gtk_revealer_set_transition_type (GTK_REVEALER (self->right),
                                      GTK_REVEALER_TRANSITION_TYPE_CROSSFADE);
    gtk_revealer_set_transition_duration (GTK_REVEALER (self->right),
                                      OVERLAY_REVEAL_ANIM_TIME);
    gtk_widget_set_halign (self->right, GTK_ALIGN_END);
    gtk_widget_set_valign (self->right, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start (self->right, 12);
    gtk_widget_set_margin_end (self->right, 12);
    gtk_overlay_add_overlay (self->overlay, self->right);

    /* randomize revealer */
    self->randomize = gtk_revealer_new ();
    gtk_revealer_set_transition_type (GTK_REVEALER (self->randomize),
                                      GTK_REVEALER_TRANSITION_TYPE_CROSSFADE);
    gtk_revealer_set_transition_duration (GTK_REVEALER (self->randomize),
                                      OVERLAY_REVEAL_ANIM_TIME);
    gtk_widget_set_halign (self->randomize, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (self->randomize, GTK_ALIGN_END);
    gtk_widget_set_margin_bottom (self->randomize, 12);
    gtk_overlay_add_overlay (self->overlay, self->randomize);

    /* next button */
    GtkWidget *button = gtk_button_new_from_icon_name ("go-next-symbolic", GTK_ICON_SIZE_BUTTON);
    gtk_container_add (GTK_CONTAINER (self->right), button);
    gtk_widget_set_tooltip_text (button, "Go to the next XKCD");
    gtk_style_context_add_class (gtk_widget_get_style_context (button),
                                 GTK_STYLE_CLASS_OSD);

    /* previous button */
    button = gtk_button_new_from_icon_name ("go-previous-symbolic", GTK_ICON_SIZE_BUTTON);
    gtk_container_add (GTK_CONTAINER (self->left), button);
    gtk_widget_set_tooltip_text (button, "Go to the previous XKCD");
    gtk_style_context_add_class (gtk_widget_get_style_context (button),
                                 GTK_STYLE_CLASS_OSD);

    /* randomize button */
    button = gtk_button_new_from_icon_name ("view-refresh", GTK_ICON_SIZE_BUTTON);
    gtk_container_add (GTK_CONTAINER (self->randomize), button);
    gtk_widget_set_tooltip_text (button, "randomize");
    gtk_style_context_add_class (gtk_widget_get_style_context (button),
                                 GTK_STYLE_CLASS_OSD);

    g_signal_connect (self->image_event, "motion-notify-event",
                      G_CALLBACK (motion_notify_event),
                      self);

    /* don't hide buttons when above */
    gtk_widget_add_events (GTK_WIDGET (self->overlay),
                           GDK_ENTER_NOTIFY_MASK);

    g_signal_connect (self->overlay, "enter-notify-event",
                      G_CALLBACK (enter_overlay_cb),
                      self);
    gtk_stack_set_visible_child_name (GTK_STACK (self), "spinner");
}
