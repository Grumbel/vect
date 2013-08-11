// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include "gfx_lib.h"
#include "zoom.hh"

double Zoom::zoom_f = 1.0;
int    Zoom::zoom_x =   0;
int    Zoom::zoom_y =   0;

void
Zoom::set_zoom(int x)
{
  zoom_f = static_cast<int>(640.0 / x); 
}

void
Zoom::set_zoom_x(int x)
{
  zoom_x = x;
}

void
Zoom::set_zoom_y(int y)
{
  zoom_y = y;
}

int
Zoom::zoom(double a)
{
  return (int)(a * zoom_f);
}

int
Zoom::zmouse_x()
{
  return zoom(mouse_x);
}

int
Zoom::zmouse_y()
{
  return zoom(mouse_y);
}

void
Zoom::zget_mouse_mickeys(int* x, int* y)
{
  get_mouse_mickeys(x, y);
  zoom(*x);
  zoom(*y);
}

void
Zoom::zrect(BITMAP* scr, int x1, int y1, int x2, int y2, int color)
{
  rect(scr,
       zoom(x1 + zoom_x), zoom(y1 + zoom_y),
       zoom(x2 + zoom_x), zoom(y2 + zoom_y),
       color);
}

void
Zoom::zrectfill(BITMAP* scr, int x1, int y1, int x2, int y2, int color)
{
  rectfill(scr,
       zoom(x1 + zoom_x), zoom(y1 + zoom_y),
       zoom(x2 + zoom_x), zoom(y2 + zoom_y),
       color);
}

void
Zoom::zline(BITMAP* scr, int x1, int y1, int x2, int y2, int color)
{
  line(scr,
       zoom(x1 + zoom_x), zoom(y1 + zoom_y),
       zoom(x2 + zoom_x), zoom(y2 + zoom_y),
       color);
}

void
Zoom::zcircle(BITMAP* scr, int x, int y, int radius, int color)
{
  circle(scr, zoom(x + zoom_x), zoom(y + zoom_y), zoom(radius), color);
}

void
Zoom::zcirclefill(BITMAP* scr, int x, int y, int radius, int color)
{
  circlefill(scr, zoom(x + zoom_x), zoom(y + zoom_y), zoom(radius), color);
}

void
Zoom::zpolygon(BITMAP* scr, int vertices, int* points, int color)
{
  if (zoom_f != 1) {
    int *zpoints;
    
    zpoints = new int[2 * vertices];
    
    for(int i=0; i < vertices; ++i) {
      zpoints[2*i]   = zoom(points[2*i]   + zoom_x);
      zpoints[2*i+1] = zoom(points[2*i+1] + zoom_y);
    }

    polygon(scr, vertices, zpoints, color);

    delete[] zpoints;

  } else {
    polygon(scr, vertices, points, color);
  }
}

void
Zoom::zputpixel(BITMAP* scr, int x, int y, int color)
{
#if HAVE_LIBALLEG
  putpixel(scr, x + zoom_x, y + zoom_y, color);
#elif HAVE_LIBGGI2D
  ggiPutPixel(scr, x, y, color);
#endif
}

int
Zoom::zgetpixel(BITMAP* scr, int x, int y)
{
#if HAVE_LIBALLEG
  return getpixel(scr, x, y);
#endif
}

/* EOF */
