// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <stdio.h>
#include <allegro.h>

#include "text.hh"

Text::Text(int col)
{
  color = col;
  //  str.push_back("Sample");
}

Text::~Text()
{
}

void
Text::draw(BITMAP *scr)
{
  //  char s[str[0].size()];
  //  strcpy(s, str[0].c_str());
  //  textout_ex(scr, font, s, x_pos, y_pos, color, -1);
}

void
Text::move(int x, int y)
{
  x_pos += x;
  y_pos += y;
}

void
Text::create_new()
{
  while(mouse_b & 1);
  while(!(mouse_b & 1));
  while(mouse_b & 1);
  
  x_pos = mouse_x;
  y_pos = mouse_y;

  /*
  int c;
  for(int i=0; ; ++i) {
    c = readkey() & 0xff;
  }*/
}

void
Text::get_border()
{
  x1 = x_pos - 2;
  y1 = y_pos - 2;
  //  x2 = x_pos + 8 * str[0].size();
  y2 = y_pos + 9;
}

void
Text::save(FILE *out)
{
  fprintf(out, "[Text]\n");
  //  fprintf(out, "%d %s\n\n", color, str[0].c_str());
}

void
Text::load(FILE *in)
{
  char buf[256];
  fscanf(in, "%d %[^\n]", &color, buf);

  //  str[0] = buf;
}

void
Text::draw_vertex()
{
}

void
Text::del_vertex()
{
}

void
Text::draw_col(BITMAP *scr, int col)
{
  //  char s[str[0].size()];
  //  strcpy(s, str[0].c_str());
  
  //  for(unsigned int i = 0; i < str.size(); ++i) 
  //    textout_ex(scr, font, s, x_pos, y_pos + 9*i, col, -1);
}

void
Text::scale()
{
}

void
Text::rotate()
{
}

void
Text::show_vertex()
{
}

void
Text::move_vertex()
{
}

void
Text::copy(Shape*)
{
}

void
Text::set_color(int col)
{
  color = col;
}

Shape*
Text::copy()
{
  return reinterpret_cast<Shape*>(0);
}

/* EOF */
