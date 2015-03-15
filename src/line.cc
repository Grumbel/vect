// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

//#include <iostream.h>
#include <stdio.h>
#include "gfx_lib.h"

#include "add_aleg.hh"
#include "defs.hh"

#include "shape.hh"
#include "line.hh"

Line::~Line()
{
  free(p);
  //cerr << "Line destroyed" << std::endl;
}

Line::Line(int col)
{
  type = LINE;
  color = col;

  p = (Point*)malloc(sizeof(*p));
  last_point = 0;
}

Line::Line()
{
  Line(255);
}

void
Line::set_color(int col)
{
  color = col;
}

void
Line::move(int x, int y)
{
  for(int i=0; i<last_point; ++i) {
    p[i].x += x;
    p[i].y += y;
  }
}

void
Line::create_new()
{
  int mx = mouse_x, my = mouse_y;
  int temp_x, temp_y;

  while(!mouse_button1_pressed());
  create_new_dot(mouse_x, mouse_y);
  temp_x = mouse_x;
  temp_y = mouse_y;

  show_mouse(NULL);
  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);

  line(screen, p[last_point - 1].x, p[last_point - 1].y,
       mouse_x, mouse_y, 255);

  while(!mouse_button2_pressed())
    {
      if (mx != mouse_x || my != mouse_y || mouse_b & 1)
	{
	  mx = mouse_x;
	  my = mouse_y;

	  line(screen, p[last_point - 1].x, p[last_point - 1].y,
	       mx, my, LINE_COLOR);
	  line(screen, p[last_point - 1].x, p[last_point - 1].y,
	   temp_x, temp_y, LINE_COLOR);

	  if (mouse_button1_pressed())
	    create_new_dot(mx, my);

	  temp_x = mx;
	  temp_y = my;
	}
    }

  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
  show_mouse(screen);
}

void
Line::create_new_dot(int x, int y)
{
  p = (Point*)realloc(p, (2+last_point)*sizeof(*p));
  p[last_point].x = x;
  p[last_point].y = y;
  ++last_point;
}

void
Line::draw_rect(BITMAP *scr)
{
  int x1 = p[0].x, y1 = p[0].y, x2 = p[0].x, y2 = p[0].y;

  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);

  for(int i=0; i<last_point; ++i) {

    if (p[i].x < x1)
      x1 = p[i].x;
    if (p[i].x > x2)
      x2 = p[i].x;
    if (p[i].y < y1)
      y1 = p[i].y;
    if (p[i].y > y2)
      y2 = p[i].y;

  }

  rect(scr, x1-1, y1-1, x2+1, y2+1, LINE_COLOR);

  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
}

void
Line::draw_col(BITMAP *scr, int col)
{
  for(int i=1; i<last_point; ++i) {
    line(scr, p[i-1].x, p[i-1].y, p[i].x, p[i].y, col);
  }
}

void
Line::draw(BITMAP *scr)
{
  draw_col(scr, color);
}

void
Line::save(FILE *out)
{
  fprintf(out, "[Line]\n");
  fprintf(out, "Color: %d\n", color);

  for(int i=0; i<last_point; ++i) {
    fprintf(out, "%d %d\n", p[i].x, p[i].y);
  }

  fprintf(out, "\n");
}

void
Line::load(FILE *in)
{
  int d=1;

  type = LINE;
  p = (Point *)malloc(sizeof(Point) * 15);
  fscanf(in, "Color: %d\n", &color);

  for(last_point  = 0;
      fscanf(in, "%d %d\n", &p[last_point].x, &p[last_point].y) == 2;
      ++last_point)
    {
      if (last_point > 14*d)
	{
	  p = (Point *)realloc(p, sizeof(Point) * 15 * d);
	  ++d;
	}
    }
}


void
Line::draw_vertex(void)
{
}

void
Line::scale(void)
{
}

void
Line::rotate(void)
{
}

void
Line::show_vertex(void)
{
}

void
Line::move_vertex(void)
{
}

void
Line::copy(Shape*)
{
}

void
Line::get_border()
{
}

/* EOF */
