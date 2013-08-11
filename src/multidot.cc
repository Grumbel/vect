// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <stdio.h>
#include <allegro.h>
#include "add_aleg.hh"
#include "multidot.hh"
#include "defs.hh"

void Multi_Dot::show_vertex()
{
  int i = get_vertex();

  if (i != -1) {
    show_mouse(NULL);
    vertex(screen, p[i].x, p[i].y, 5, VERTEX_HCOLOR);
    show_mouse(screen);
  }
}

int Multi_Dot::get_vertex()
{  
  for(unsigned int i=0; i < p.size(); ++i) {
    if    (mouse_x > p[i].x - 4
	   && mouse_x < p[i].x + 4
	   && mouse_y > p[i].y - 4
	&& mouse_y < p[i].y + 4)
      {
	return i;
      }
  }
  return -1;
}

void Multi_Dot::draw_vertex()
{
  for(unsigned int i=0; i < p.size(); ++i)
    vertex(screen, p[i].x, p[i].y, 5, VERTEX_COLOR);
}

bool Multi_Dot::is_in(int rect_x1, int rect_y1, int rect_x2, int rect_y2)
{
  get_border();

  //  cerr << x1 << x2 << y1 << y2 << endl;
  if (rect_x1 > rect_x2)
    swap(rect_x1, rect_x2);
  if (rect_y1 > rect_y2)
    swap(rect_y1, rect_y2);
  
  if (x1 > rect_x1 && x2 < rect_x2
      && y1 > rect_y1 && y2 < rect_y2)
    return true;

  return false;
  //  return rect_col(rect_x1, rect_y1, rect_x2, rect_y2, x1, y1, x2, y2);
}

void Multi_Dot::get_border()
{
  x1 = x2 = p[0].x;
  y1 = y2 = p[0].y;
  
  for(unsigned int i=0; i < p.size(); ++i) {  
    if (p[i].x < x1)
      x1 = p[i].x;
    if (p[i].x > x2)
      x2 = p[i].x;
    if (p[i].y < y1)
      y1 = p[i].y;
    if (p[i].y > y2)
      y2 = p[i].y;
  }
}
				 
/* EOF */
