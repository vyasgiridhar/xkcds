/* xkcd.h
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

#ifndef XKCD_H
#define XKCD_H

#include <glib-object.h>
#include "xkcds-image.h"
#include <libsoup/soup.h>
#include <json-glib-1.0/json-glib/json-glib.h>

G_BEGIN_DECLS

#define XKCD_URL "http://xkcd.com/%d/info.0.json"

#define XKCD_TYPE_OBJECT (xkcd_object_get_type())

G_DECLARE_DERIVABLE_TYPE (Xkcd, xkcd_object, XKCD, OBJECT, GObject)

struct _XkcdClass
{
    GObjectClass parent;

    void (* loaded) (Xkcd, XkcdsImage);
};

Xkcd *xkcd_new (void);

void
xkcd_object_load_async (Xkcd               *self,
                        int                *movement,
                        GCancellable       *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer            data);


G_END_DECLS

#endif /* XKCD_H */

