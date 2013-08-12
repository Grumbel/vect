// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <cstdio>
#include "rect.hh"

void swap(int &a, int&b)
{
  int temp = a;
  a = b;
  b = temp;
}

Rect::Rect(int col)
{
  color = col;
}

Rect::~Rect()
{
}

void
Rect::draw(BITMAP* scr)
{
  rectfill(scr, x1, y1, x2, y2, color);
}

void
Rect::move(int x, int y)
{
  x1 += x;
  x2 += x;
  y1 += y;
  y2 += y;
}

void
Rect::create_new()
{
  while(mouse_b & 1);
  while(!(mouse_b & 1));
  while(mouse_b & 1);

  int mx = mouse_x, my = mouse_y;
  int temp_mx = mouse_x, temp_my = mouse_y;
  x1 = mx;
  y1 = my;

  show_mouse(NULL);
  rect(screen, x1, y1, temp_mx, temp_my, LINE_COLOR);
  show_mouse(screen);
  
  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);

  while(!(mouse_b & 1)) {
    if (mx != mouse_x || my != mouse_y) {
      mx = mouse_x;
      my = mouse_y;
      show_mouse(NULL);
      rect(screen, x1, y1, temp_mx, temp_my, LINE_COLOR);
      rect(screen, x1, y1, mx, my, LINE_COLOR);
      show_mouse(screen);
      temp_mx = mx;
      temp_my = my;
    }
  }

  x2 = mx;
  y2 = my;

  if (x1 > x2)
    swap(x1, x2);
  if (y1 > y2)
    swap(y1, y2);
    
  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
  
  while(mouse_b & 1);
}
/*
void
Rect::draw_rect(BITMAP* scr)
{
  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);
  rect(scr, x1, y1, x2, y2, LINE_COLOR);
  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
}
*/
void
Rect::save(FILE *out)
{
  fprintf(out, "[Rect]\n");
  fprintf(out, "%d %d %d %d %d\n\n", color, x1, y1, x2, y2);
}

void
Rect::load(FILE *in)
{
  fscanf(in, "%d %d %d %d %d\n\n", &color, &x1, &y1, &x2, &y2);
}

void
Rect::draw_col(BITMAP *scr, int col)
{
  rectfill(scr, x1, y1, x2, y2, col);
}

void
Rect::scale()
{
}

void
Rect::rotate()
{
}
/*
void
Rect::set_color(int)
{
}
*/
Shape*
Rect::copy()
{
  Rect* copy;

  copy = new Rect;

  copy->x1 = x1;
  copy->x2 = x2;
  copy->y1 = y1;
  copy->y2 = y2;
  copy->color = color;

  return static_cast<Shape*>(copy);    
}

/* EOF */
