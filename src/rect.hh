// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef RECT_HH
#define RECT_HH

#include <stdio.h>
#include <allegro.h>
#include "shape.hh"
#include "defs.hh"

class Rect : public Shape
{
public:
  Rect(int col=255);
  ~Rect();

  void   draw(BITMAP *scr);
  void   move(int x, int y);
  void   create_new();
  //  void   draw_rect(BITMAP *scr);
  void   save(FILE *out);
  void   load(FILE *in);
  void   draw_col(BITMAP *scr, int col);
  void   scale();
  void   rotate();
  // void   set_color(int);
  Shape* copy();
};

#endif // #ifndef RECT_HH

/* EOF */
