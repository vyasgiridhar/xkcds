/* xkcd-api.h
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

#ifndef XKCD_API_H
#define XKCD_API_H

#include "xkcd.h"
#include "xkcds-image.h"
#include <glib-object.h>
#include <libsoup/soup.h>
#include <json-glib-1.0/json-glib/json-glib.h>

G_BEGIN_DECLS

#define XKCD_URL "http://xkcd.com/%d/info.0.json"
#define XKCD_TYPE_API (xkcd_api_get_type())

G_DECLARE_FINAL_TYPE (XkcdApi, xkcd_api, XKCD, API, GObject)

XkcdApi *xkcd_api_new (void);

void
xkcd_api_get_random (XkcdApi *self, XkcdsImage *image);

G_END_DECLS

#endif /* XKCD_API_H */
