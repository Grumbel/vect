// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef TEXT_HH
#define TEXT_HH

#include <vector>
#include <string>
#include "shape.hh"

class Text : public Shape
{
public:
  Text(int col = 255);
  ~Text();
  Shape_Type type;
  
  void   draw(BITMAP *scr);
  void   move(int x, int y);
  void   create_new();
  void   save(FILE *out);
  void   load(FILE *in);
  void   draw_vertex();
  void   del_vertex();
  void   draw_col(BITMAP *scr, int col);
  void   scale();
  void   rotate();
  void   show_vertex();
  void   move_vertex();
  void   copy(Shape*);
  void   set_color(int);
  void   get_border();
  Shape* copy();
protected:
  int    x_pos, y_pos;
  int    color;
  //  vector<string> str;
};

#endif TEXT_HH

/* EOF */
