/*
 *  gensio - A library for abstracting stream I/O
 *  Copyright (C) 2018  Corey Minyard <minyard@acm.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 */

#ifndef GENSIO_SELECTOR_H
#define GENSIO_SELECTOR_H

#include <gensio/gensio.h>
#include <gensio/selector.h>

struct gensio_os_funcs *gensio_selector_alloc(struct selector_s *sel,
					      int wake_sig);

/* For testing, do not use in normal code. */
void gensio_sel_exit(int rv);

#endif /* GENSIO_SELECTOR_H */
