// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <iostream>
#include <stdio.h>
#include <allegro.h>
#include <math.h>

#include "add_aleg.hh"
#include "shape.hh"

Shape::Shape(const Shape& s)
{
  //int x1, x2, y1, y2;
  //int color;

  x1 = s.x1;
  x2 = s.x2;
  y1 = s.y1;
  y2 = s.y2;
  color =  s.color;
}

void
Shape::draw_unfilled(BITMAP* scr)
{
  Shape::draw_rect(scr);
}

void
Shape::draw_rect(BITMAP *scr)
{
  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);

  rect(scr, x1, y1, x2, y2, LINE_COLOR);

  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
}

bool
Shape::is_in(int rect_x1, int rect_y1, int rect_x2, int rect_y2)
{
  if (x1 > rect_x1 && x2 < rect_x2
      && y1 > rect_y1 && y2 < rect_y2)
    return true;

  return false;
}

void
Shape::
set_color(int col)
{
  color = col;
}

// Object
/*
Object::Object() {type = OBJECT;}
void Object::draw(BITMAP *scr){}
void Object::move(int x, int y){}
void Object::create_new(){}
void Object::draw_rect(BITMAP *scr){}
void Object::save(FILE *out){}
void Object::load(FILE *in){}
void Object::draw_vertex(){}
void Object::draw_col(BITMAP *scr, int col){}
void Object::scale(){}
void Object::rotate(){}
void Object::show_vertex(){}
void Object::move_vertex(){}
void Object::copy(Object*)
{
  std::cerr << "In Object Const" << std::endl;
}*/

/* EOF */
