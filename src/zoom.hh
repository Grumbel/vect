// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <allegro.h>

class Zoom {
private:
public:
  static double zoom_f;
  static int zoom_x;
  static int zoom_y;
public:
  static void zrect(BITMAP* scr, int x1, int y1, int x2, int y2, int color);
  static void zrectfill(BITMAP* scr, int x1, int y1, int x2, int y2, int color);
  static void zline(BITMAP* scr, int x1, int y1, int x2, int y2, int color);
  static void zcircle(BITMAP* scr, int x, int y, int radius, int color);
  static void zcirclefill(BITMAP* scr, int x, int y, int radius, int color);
  static void zpolygon(BITMAP* scr, int vertices, int* points, int color);
  static void zputpixel(BITMAP* scr, int x, int y, int color);
  static int  zgetpixel(BITMAP* scr, int x, int y);

  static void set_zoom(int);
  static void set_zoom_x(int);
  static void set_zoom_y(int);

  static int zmouse_x();
  static int zmouse_y();
  static void zget_mouse_mickeys(int*, int*);

  static int zoom(double a);
};

/* EOF */
