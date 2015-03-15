// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef DOT_HH
#define DOT_HH

#include "shape.hh"
#include "multidot.hh"

class Dot : public Multi_Dot
{
public:
  Dot(int col=255);
  Dot(Dot&);
  ~Dot();

  void create_new();
  void move(int x, int y);
  void draw_rect(BITMAP*);
  void draw(BITMAP*);
  void save(FILE*);
  void load(FILE*);
  //void draw_vertex(){};
  void del_vertex();
  void draw_col(BITMAP*, int);
  void scale();
  void stretch_draw(BITMAP*, int);
  void rotate_draw(BITMAP*, int);
  //void show_vertex();
  Dot* copy();
  void move_vertex();
  void rotate();
  void copy(Shape*){}
  void set_color(int);
private:
  bool is_cached;
  BITMAP *cache_scr;
};

#endif // #ifndef DOT_HH

/* EOF */
