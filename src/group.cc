// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <stdio.h>
#include <vector>
#include "shape.hh"
#include "group.hh"

Group::Group()
{
  type = GROUP;
}

Group::Group(const std::vector<Shape*> &b)
{
  type = GROUP;
  a = b;

  for(unsigned int i=0; i < a.size(); ++i) {
    //   if (x1_pos > a[i].
  }
}

Group::~Group()
{
}

void
Group::draw(BITMAP* scr)
{
  for(unsigned int i=0; i < a.size(); ++i) {
    a[i]->draw(scr);
  }
}

void
Group::draw_rect(BITMAP* scr)
{
  for(unsigned int i=0; i < a.size(); ++i) {
    a[i]->draw_rect(scr);
  }
}

void
Group::move(int x, int y)
{
  for(unsigned int i=0; i < a.size(); ++i) {
    a[i]->move(x,y);
  }
}

void
Group::load(FILE* in)
{
  unsigned int size;
  fscanf(in, "%d\n", &size);
  a.reserve(size);
  for(unsigned int i=0; i < size; ++i) {
    a[i]->load(in);
  }
}

void
Group::save(FILE* out)
{
  fprintf(out, "[Group]\n");
  fprintf(out, "%d\n", a.size());
  
  for(unsigned int i=0; i < a.size(); ++i) {
    a[i]->save(out);
  }
}


void
Group::draw_col(BITMAP* scr, int col)
{
  for(unsigned int i=0; i < a.size(); ++i) {
    a[i]->draw_col(scr, col);
  }
}

void
Group::scale()
{
  for(unsigned int i=0; i < a.size(); ++i) {
    a[i]->scale();
  }
}

void
Group::rotate()
{
  for(unsigned int i=0; i < a.size(); ++i) {
    a[i]->rotate();
  }
}

bool
Group::is_in(int x1, int y1, int x2, int y2)
{
  if (x1 < x1_pos && x2 > x2_pos
      && y1 < y1_pos && y2 > y2_pos)
    return true;
  
  return false;
}


Shape*
Group::copy()
{
  return this;
}

unsigned int
Group::size()
{
  return a.size();
}

Shape*
Group::at(unsigned int i)
{
  return a[i];
}

Shape*
Group::operator[](unsigned int i)
{
  return a[i];
}

void
Group::get_border()
{
}

/* EOF */
