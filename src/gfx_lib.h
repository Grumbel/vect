// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef GFX_LIB_H
#define GFX_LIB_H

#if HAVE_LIBALLEG
#  include <allegro.h>
#elif HAVE_LIBGGI2D
#  include <ggi/ggi2d.h>
#  define BITMAP ggi_visual_t
#endif

#endif

/* EOF */
