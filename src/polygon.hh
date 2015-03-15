// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef POLYGON__HH
#define POLYGON__HH

#include "multidot.hh"

class Polygon : public Multi_Dot
{
public:
  Polygon(int col=255);
  Polygon(const Polygon& a);
  ~Polygon();

  void create_new();
  void move(int x, int y);
  void draw_rect(BITMAP*);
  void draw(BITMAP*);
  void save(FILE*);
  void load(FILE*);
  void draw_vertex();
  void del_vertex();
  void draw_col(BITMAP*, int);
  void scale();
  void stretch_draw(BITMAP*, int);
  void rotate_draw(BITMAP*, int);
  void show_vertex();
  void move_vertex();
  void rotate();
  void copy(Shape*);
  Shape* copy();
  void set_color(int);
private:
  //Point *p;
  int   *p_format;
  //int   last_point;
  bool  format_is_up_to_date;

  //void get_points(Point* p, int last_p);
  void create_new_dot(int x, int y);
  void create_format();
  int  get_vertex();
};

#endif // ifndef POLYGON_HH

/* EOF */
