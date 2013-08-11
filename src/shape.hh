// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef SHAPE_HH
#define SHAPE_HH

#include <stdio.h>
#include <allegro.h>
#include "defs.hh"
 
class Shape
{
public:
  Shape() {};
  Shape(const Shape&);
  virtual ~Shape(){};
  Shape_Type type;
    
  virtual void   draw(BITMAP* scr) = 0;
  virtual void   draw_unfilled(BITMAP* scr);
  virtual void   move(int x, int y) = 0;
  virtual void   create_new(){};
  virtual void   draw_rect(BITMAP *scr);
  virtual void   save(FILE *out) = 0;
  virtual void   load(FILE *in) = 0;
  virtual void   draw_vertex() {};
  virtual void   del_vertex() {};
  virtual void   draw_col(BITMAP *scr, int col) = 0;
  virtual void   scale() = 0;
  virtual void   rotate() = 0;
  virtual void   show_vertex(){};
  virtual void   move_vertex(){};
  virtual void   copy(Shape*){};
  virtual void   set_color(int);
  virtual Shape* copy(){return reinterpret_cast<Shape*>(0);}
  virtual bool   is_in(int, int, int, int);
protected:
  int x1, x2, y1, y2;
  int color;
  virtual void get_border() {};
};

#endif // #ifndef OBJECTS_HH

/* EOF */
