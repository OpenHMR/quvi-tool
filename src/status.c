/* quvi
 * Copyright (C) 2012,2013  Toni Gundogdu <legatvs@gmail.com>
 *
 * This file is part of quvi <http://quvi.sourceforge.net/>.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General
 * Public License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <glib/gi18n.h>
#include <quvi.h>

#include "lutil.h"
#include "sig.h"

static const lutil_cb_printerr perr = lutil_print_stderr_unless_quiet;

static void _saymsg(const gchar *m, const gboolean with_time)
{
  gsize l, i;
  gchar *s;

  if (with_time == TRUE)
    {
      GDateTime *n;
      gchar *t;

      n = g_date_time_new_now_local();
      t = g_date_time_format(n, "%T");
      g_date_time_unref(n);

      if (t == NULL)
        return;

      s = g_strdup_printf("[%s] %s", t, m);
      g_free(t);
    }
  else
    s = g_strdup(m);

  l = sigwinch_term_spaceleft(strlen(s));
  perr("%s", s);
  g_free(s);

  for (i=0; i<l; ++i)
    perr(" ");
  perr("\r");
}

static void _say(const gchar *m)
{
  _saymsg(m, TRUE);
}

static void _done()
{
  _saymsg("", FALSE);
}

static void _fetch(const quvi_word type, const gpointer p)
{
  if (type != QUVI_CALLBACK_STATUS_DONE)
    _say(_("fetch <url> ..."));
  else
    _done();
}

static void _resolve(const quvi_word type)
{
  if (type != QUVI_CALLBACK_STATUS_DONE)
    _say(_("resolve <url> ..."));
  else
    _done();
}

static void _query_metainfo(const quvi_word type)
{
  if (type != QUVI_CALLBACK_STATUS_DONE)
    _say(_("metainfo <url> ..."));
  else
    _done();
}

QuviError cb_status(glong param, gpointer p)
{
  quvi_word status, type;

  status = quvi_loword(param);
  type = quvi_hiword(param);

  switch (status)
    {
    case QUVI_CALLBACK_STATUS_HTTP_QUERY_METAINFO:
      _query_metainfo(type);
      break;
    case QUVI_CALLBACK_STATUS_RESOLVE:
      _resolve(type);
      break;
    case QUVI_CALLBACK_STATUS_FETCH:
      _fetch(type, p);
      break;
    default:
      g_warning("[%s] unknown quvi status code 0x%x", __func__, status);
      break;
    }
  return (QUVI_OK);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
