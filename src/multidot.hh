// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef MULTIDOT_HH
#define MULTIDOT_HH

#include <vector>
#include "shape.hh"
#include "defs.hh"

class Multi_Dot :  public Shape
{
public:
  void show_vertex(void);
  void draw_vertex(void);
  int  get_vertex(void);
  bool is_in(int, int, int, int);
  virtual void get_border();
protected:
  std::vector<Point> p;
};

#endif

/* EOF */
