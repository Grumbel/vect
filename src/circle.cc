// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <iostream>
#include <stdio.h>
#include <allegro.h>

#include "zoom.hh"
#include "defs.hh"
#include "add_aleg.hh"
#include "status.hh"

#include "shape.hh"
#include "circle.hh"

extern Status_Line status_line;

Circle::Circle(int col)
{
  type = CIRCLE;
  color = col;
  filled = TRUE;
}

Circle::~Circle()
{
}

void
Circle::get_border()
{
  x2 = x + radius;
  x1 = x - radius;
  y2 = y + radius;
  y1 = y - radius;
}

void
Circle::move(int xm, int ym)
{
 x += xm;
 y += ym;
}

void
Circle::set_color(int col)
{
  color = col;
}

void
Circle::draw_rect(BITMAP *scr)
{
  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);

  Zoom::zrect(scr, x-radius-1, y-radius-1, x+radius+1, y+radius+1, LINE_COLOR);

  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
}

void
Circle::draw_col(BITMAP *scr, int col)
{
  Zoom::zcirclefill(scr, x, y, radius, col);
}

void
Circle::draw(BITMAP *scr)
{
  if (filled)
    Zoom::zcirclefill(scr, x, y, radius, color);
  else
    Zoom::zcircle(scr, x, y, radius, color);
}

void
Circle::create_new()
{
  char str[256];
  int  mx=0, mx1, my1;
  int  temp_radius=0;

  while (!mouse_button1_pressed());

  x = Zoom::zmouse_x();
  y = Zoom::zmouse_y();

  show_mouse(NULL);

  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);
  get_mouse_mickeys(&mx1, &my1);
  Zoom::zcircle(screen, x, y, temp_radius, LINE_COLOR);

  while(!mouse_button1_pressed())
    {
      get_mouse_mickeys(&mx1, &my1);

      if (mx1 != 0)
	{
	  mx += mx1;

	  radius = abs(mx);

	  circle(screen, x, y, radius, LINE_COLOR);
	  circle(screen, x, y, temp_radius, LINE_COLOR);
	  temp_radius = radius;
	  sprintf(str, "Radius %4d X: %4d Y: %4d", radius, x, y);
	  status_line.print(str);
	}
    }

  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
  show_mouse(screen);
  get_border();
}

void
Circle::save(FILE *out)
{
  fprintf(out, "[Circle]\n");
  fprintf(out, "Color: %d\n", color);
  fprintf(out, "Radius: %d\n", radius);
  fprintf(out, "%d %d\n", x, y);
  fprintf(out, "\n");
}

void
Circle::load(FILE *in)
{
  if (fscanf(in, "Color: %d\nRadius: %d\n", &color, &radius) != 2)
    {
      fprintf(stderr, "Konnte Circle nicht laden\n");
      exit(1);
    }

  if (fscanf(in, "%d %d\n\n", &x, &y) != 2)
    {
      fprintf(stderr, "Konnte Circle nicht laden\n");
      exit(1);
    }
  type = CIRCLE;
}

void
Circle::copy(Shape* b)
{
  Circle *c = static_cast<Circle*>(b);

  //  std::cerr << "In Copy" << std::endl;
  radius = c->radius;
  x = c->x;
  y = c->y;
  type = c->type;
  // std::cerr << "out Copy" << std::endl;
}

void
Circle::draw_vertex(void)
{
}

void
Circle::scale(void)
{
  get_border();
}

void
Circle::rotate(void)
{
}

void
Circle::show_vertex(void)
{
}

void
Circle::move_vertex(void)
{
}

/* EOF */
