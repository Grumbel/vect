// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <stdio.h>
#include "gfx_lib.h"
#include "zoom.hh"
#include "add_aleg.hh"
#include "defs.hh"

//#include "shape.hh"
//#include "multidot.hh"
#include "dot.hh"

/*Dot::Dot()
{
  is_cached = false;
  cache_scr = 0;
}*/
#include "status.hh"
extern Status_Line status_line;

Dot::Dot(int col)
{
  is_cached = false;
  cache_scr = 0;
  color = col;
}

Dot* Dot::copy()
{
  Dot* a = new Dot();
  
  a->x1 = x1;
  a->x2 = x2;
  a->y1 = y1;
  a->y2 = y2;
  a->p  = p;
  a->is_cached = false;
  a->cache_scr = 0;
  a->color = color;

  return a;
}

Dot::~Dot()
{
}

void Dot::set_color(int col)
{
  color = col;
  is_cached = false;
}

void Dot::create_new()
{
  int mx = 0;
  int my = 0;
  int i=0;
  Point p1;
  char str[256];

  while(!(mouse_b & 2)) {
    if ((mouse_b & 1) && (mouse_x != mx || mouse_y != my)) {
      show_mouse(NULL);

      p1.x = mouse_x;
      p1.y = mouse_y;

      p.push_back(p1);

      Zoom::zputpixel(screen, p[i].x, p[i].y, 255);

      sprintf(str, "Number of Dots: %6d", (int)p.size());
      status_line.print(str);

      i++;

      show_mouse(screen);

      mx = mouse_x;
      my = mouse_y;
    }
  }
  while(mouse_b & 2);
  draw(screen);
}

void Dot::move(int x, int y)
{
  for (unsigned int i=0; i < p.size(); ++i) {
    p[i].x += x;
    p[i].y += y;
  }
  is_cached = false;
}

void Dot::draw_col(BITMAP* scr, int col)
{
  for (unsigned int i=0; i < p.size(); ++i)
    Zoom::zputpixel(scr, p[i].x, p[i].y, col);
}

void Dot::draw(BITMAP *scr)
{
  if (is_cached) {
    draw_sprite(scr, cache_scr, 0,0);
  } else {

    if (cache_scr)
      destroy_bitmap(cache_scr);
    
    cache_scr = create_bitmap(SCREEN_W, SCREEN_H);
    
    clear(cache_scr);
    
    for (unsigned int i=0; i < p.size(); ++i)
      Zoom::zputpixel(cache_scr, p[i].x, p[i].y, color);
    
    draw_sprite(scr, cache_scr, 0,0);

    is_cached = true;
  } 
}

void Dot::draw_rect(BITMAP *scr)
{
  int x1 = p[0].x, x2 = p[0].x;
  int y1 = p[0].y, y2 = p[0].y;
  
  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);
  get_border();  
  Zoom::zrect(scr, x1-1, y1-1, x2+1, y2+1, LINE_COLOR);
  
  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
}

void Dot::save(FILE* out)
{
  fprintf(out, "[Dot]\n");
  fprintf(out, "%d\n", color);
  
  for(unsigned int i=0; i < p.size(); ++i) {
    fprintf(out, "%d %d\n", p[i].x, p[i].y);
  }
  fprintf(out, "\n");  
}

void Dot::load(FILE* in)
{
  Point point;
  fscanf(in, "%d\n", &color);

  while(fscanf(in, "%d %d\n", &point.x, &point.y) == 2) {
    p.push_back(point);
  }
  is_cached = false;
  //  std::cerr << "Dot loading is not suported" << std::endl;
  //return ;

  //////////////////////////////////////////////

  /*
  //clog << "Polgon: Trying to load a polygon" << std::endl;
  char buf[256];

  p = (Point *)malloc(sizeof(Point) * 15);

  if (p == NULL)
    {
      std::cerr << "Polygon: Out of memory!!!" << std::endl;
      exit(EXIT_FAILURE);
    }
  
  fgets(buf, 255, in);
  
  if (sscanf(buf, "Color: %d", &color) != 1) {
    std::cerr << "Polygon: Could not load color" << std::endl;
    std::cerr << buf;
    exit(EXIT_FAILURE);
  }

  for(last_point  = 0; ; ++last_point)
    {
      fgets(buf, 255, in);
      //cout << buf;
      
      if (sscanf(buf, "%d %d", &p[last_point].x, &p[last_point].y) != 2)
	break;
      
      p = (Point *)realloc(p, sizeof(Point) * (last_point + 5));
      
      if (p == NULL)
	{
	  std::cerr << "Polygon: Out of Memory!!!" << std::endl;
	  exit(EXIT_FAILURE);
	}
      
    }

  p_format = 0;
  type = POLYGON;
  format_is_up_to_date = false;
  // clog << "Polygon: successfull loaded" << std::endl;
  */
}

void Dot::del_vertex()
{
}

void Dot::move_vertex()
{
}

void Dot::stretch_draw(BITMAP* scr, int zoom)
{
}

void Dot::rotate_draw(BITMAP*, int rot)
{
}

void Dot::rotate()
{
}

void Dot::scale()
{
}

/* EOF */
