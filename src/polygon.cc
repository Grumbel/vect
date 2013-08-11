// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <iostream>
#include <stdio.h>
#include "gfx_lib.h"
#include <math.h>

#include "zoom.hh"
#include "add_aleg.hh"
#include "shape.hh"
#include "polygon.hh"
#include "defs.hh"

// Polygon
Polygon::Polygon(int col = 255)
{
  //clog << "Polygon: constructing..." << std::endl;
  type = POLYGON;
  color = col;
  // p = NULL;
  p_format = 0;
  //  last_point = 0;
  format_is_up_to_date = FALSE;
}

Polygon::~Polygon()
{
  delete[] p_format;
}

void Polygon::set_color(int col)
{
  color = col;
}

void Polygon::draw_vertex()
{
  show_mouse(NULL);
  
  for(unsigned int i=0; i < p.size(); ++i)
    vertex(screen, p[i].x, p[i].y, 5, VERTEX_COLOR);
  
  show_mouse(screen);
}

void Polygon::create_new()
{
  int temp_x1, temp_y1;
  int temp_x2, temp_y2;
  int temp_x3, temp_y3;
  int mx = mouse_x, my = mouse_y;

  while(!mouse_button1_pressed());

  temp_x2 = mouse_x;
  temp_y2 = mouse_y;

  create_new_dot(temp_x2, temp_y2);
  
  temp_x1 = p[0].x;
  temp_y1 = p[0].y;
  temp_x3 = p[p.size() - 1].x;
  temp_y3 = p[p.size() - 1].y;

  show_mouse(NULL);
  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);
  
  while(!mouse_button2_pressed())
    {
      if (mx != mouse_x || my != mouse_y || mouse_b & 1)
	{
	  show_mouse(NULL);
	  mx = mouse_x;
	  my = mouse_y;
    
	  Zoom::zline(screen,
	       p[0].x, p[0].y,
	       mx, my,
	       LINE_COLOR);

	  Zoom::zline(screen,
	       temp_x1, temp_y1,
	       temp_x2, temp_y2,
	       LINE_COLOR);
		  
	  if (p.size() > 1)
	    {
	      Zoom::zline(screen,
		   p[p.size() - 1].x, p[p.size() - 1].y,
		   mx, my,
		   LINE_COLOR);
	      
	      Zoom::zline(screen,
		   temp_x3, temp_y3,
		   temp_x2, temp_y2,	   
		   LINE_COLOR);
	    }
	  
	  temp_x1 = p[0].x;
	  temp_y1 = p[0].y;
	  temp_x2 = mx;
	  temp_y2 = my;
	  temp_x3 = p[p.size() - 1].x;
	  temp_y3 = p[p.size() - 1].y;
    
	  if (mouse_button1_pressed())
	    {
	      if (p.size() > 1)
		{
		  Zoom::zline(screen,
		       p[p.size() - 1].x, p[p.size() - 1].y,
		       mx, my,
		       LINE_COLOR);
		 }
	      create_new_dot(mx, my);
	    }
	  show_mouse(screen);
	}
    }
  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
  show_mouse(screen);
}

void Polygon::move(int x, int y)
{
  for(unsigned int i=0; i<p.size(); ++i) {
    p[i].x += x;
    p[i].y += y;
  }
  format_is_up_to_date = FALSE;
}

void Polygon::create_new_dot(int x, int y)
{
  Point a = {x, y};

  p.push_back(a);// = (Point*)realloc(p, (2+p.size())*sizeof(*p));
  /*
  if (p == 0)
    {
      std::cerr << "Out of memory!!!" << std::endl;
      exit(1);
    }
    */
  /*
  p[p.size()].x = x;
  p[p.size()].y = y;
  ++p.size();
  */
}

void Polygon::draw_rect(BITMAP *scr = screen)
{
  int x1 = p[0].x, y1 = p[0].y, x2 = p[0].x, y2 = p[0].y;
  
  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);

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
  
  Zoom::zrect(scr, x1-1, y1-1, x2+1, y2+1, LINE_COLOR);
  
  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
}

void Polygon::create_format()
{
  //clog << "Polygon: Trying to free \"p_format\"" << std::endl;
  delete[] p_format;
  //clog << "Polygon: Successfully freed \"p_format\"" << std::endl;
  p_format = new int [(p.size()) * 2];

  // (int*)malloc(sizeof(*a)* (p.size()*2));

  for(unsigned int i=0; i<p.size(); ++i) {
    p_format[2*i] = p[i].x;
    p_format[2*i+1] = p[i].y;
  }
  
  format_is_up_to_date = true;
}

void Polygon::draw_col(BITMAP *scr, int col)
{
  if (!format_is_up_to_date) 
    create_format();
  
  Zoom::zpolygon(scr, p.size(), p_format, col); 
}

inline
void Polygon::draw(BITMAP *scr)
{
  draw_col(scr, color);
}

void Polygon::save(FILE *out)
{
  fprintf(out, "[Polygon]\n");
  fprintf(out, "Color: %d\n", color);
  
  for(unsigned int i=0; i < p.size(); ++i) {
    fprintf(out, "%d %d\n", p[i].x, p[i].y);
  }
  fprintf(out, "\n");
}

void Polygon::load(FILE *in)
{
  char buf[256];

  fgets(buf, 255, in);
  
  if (sscanf(buf, "Color: %d", &color) != 1) {
    std::cerr << "Polygon: Could not load color" << std::endl;
    std::cerr << buf;
    exit(EXIT_FAILURE);
  }

  Point p1;
  for(;;)
    {
      fgets(buf, 255, in);
      if (sscanf(buf, "%d %d", &(p1.x), &(p1.y)) != 2)
	break;
      p.push_back(p1);
    }

  p_format = 0;
  type = POLYGON;
  format_is_up_to_date = false;
}


void Polygon::scale()
{
  show_mouse(NULL);

  int mx = mouse_x;
  int scale = 0;
  int temp_scale = 0;
  
  while(!(mouse_b & 1)) {
    scale = mx - mouse_x;

    if (scale != temp_scale) {
      stretch_draw(screen, 100 - scale);
      temp_scale = scale;
    }
  }

  int x1 = SCREEN_W, y1 = SCREEN_H;
  
  for(unsigned int i=0; i < p.size(); ++i) {
    if (x1 > p[i].x)
      x1 = p[i].x;
    if (y1 > p[i].y)
      y1 = p[i].y;
  }

  
  for(unsigned int i=0; i < p.size(); ++i) {
    p[i].x = (p[i].x - x1) * (100 - scale) / 100 + x1;
    p[i].y = (p[i].y - y1) * (100 - scale) / 100 + y1;      
  }
 
  create_format();

  while(mouse_b & 1);
  
  show_mouse(screen);
}

void Polygon::stretch_draw(BITMAP *scr, int scale)
{
  int x1 = SCREEN_W, y1 = SCREEN_H;
  
  for(unsigned int i=0; i < p.size(); ++i) {
    if (x1 > p[i].x)
      x1 = p[i].x;
    if (y1 > p[i].y)
      y1 = p[i].y;
  }

  int *p_f = new int [p.size() * 2];
  
  for(unsigned int i=0; i < p.size(); ++i) {
    p_f[2*i]   = (p[i].x - x1) * scale / 100 + x1;
    p_f[2*i+1] = (p[i].y - y1) * scale / 100 + y1;      
  }

  //vsync();
  clear(screen);
  Zoom::zpolygon(scr, p.size(), p_f, color);

  delete[] p_f;
}

void Polygon::rotate()
{
  int mx = mouse_x;
  int temp_mouse_x = mouse_x;
  
  while (!(mouse_b & 1)) {
    if (mouse_x != temp_mouse_x) {
      //vsync();
      clear(screen);
      rotate_draw(screen, mx - mouse_x);
      temp_mouse_x = mouse_x;
    }
  }
  
  while(mouse_b & 1);
}

void Polygon::del_vertex()
{
  int vertex = get_vertex();

  if (vertex != -1)
    p.erase(p.begin() + vertex);

  format_is_up_to_date = false;
}

int Polygon::get_vertex()
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

void Polygon::show_vertex()
{
  int i = get_vertex();
  if (i != -1) {
    show_mouse(NULL);
    vertex(screen, p[i].x, p[i].y, 5, VERTEX_HCOLOR);
    show_mouse(screen);
  }
}

void Polygon::move_vertex()
{
 int mx, my;
 unsigned int i;
 
 for(i=0; i < p.size(); ++i) {
   if (mouse_x > p[i].x - 2
       && mouse_x < p[i].x + 2
       && mouse_y > p[i].y - 2
       && mouse_y < p[i].y + 2)
     {
       break; 
     }
 }
 get_mouse_mickeys(&mx, &my);
 show_mouse(NULL);

 while(mouse_b & 1 && i != p.size()) {
   get_mouse_mickeys(&mx, &my);
   
   if (mx != 0 || my != 0) {
     p[i].x += mx;
     p[i].y += my;
     format_is_up_to_date = FALSE;

     clear(screen);
     draw(screen);
   }
 }
 show_mouse(screen);
}

void Polygon::rotate_draw(BITMAP *scr, int rot)
{
  int rx, ry;
  int x1 = p[0].x, x2 = p[0].x, y1 = p[0].y, y2 = p[0].y;

  for(unsigned int i=0; i < p.size(); ++i) {
    if (x1 > p[0].x)
      x1 = p[0].x;
    
    if (x2 < p[0].x)
      x2 = p[0].x ;

    if (y1 > p[0].y)
      y1 = p[0].y;

    if (y2 < p[0].y)
      y2 = p[0].y;
  }

  rx = (x1 + x2) / 2;
  ry = (y1 + y2) / 2;

  double tan;
  double c;
  const double pi = 3.1415927;
  int a,b;

  int p_f[p.size() * 2];
  
  for(unsigned int i=0; i < p.size(); ++i) {
    a = p_f[2*i] - rx;
    b = p_f[2*i+1] - ry;

    c = sqrt(pow(a, 2) * pow(a, 2));
    
    tan = atan((double)(p_f[2*i] - rx)/(p_f[i*2]+1 - ry)); 
    tan += rot * (pi / 180);
    a = (int)sin(270.0 * pi / 180 - tan);
    b = (int)(sin(tan) * c);

    p_f[2*i] = a + rx;
    p_f[2*i+1] = b + ry;
  }

  Zoom::zpolygon(scr, p.size(), p_f, color);
}

void Polygon::copy(Shape*b)
{
  Polygon *c = static_cast<Polygon*>(b);

  //  p.size() = c->p.size();
  //  c->color = c->color;
  /*
  p = new Point[p.size()];

  for(int i=0; i < c->p.size(); ++i) {
    p[i].x = c->p[i].x;
    p[i].y = c->p[i].y;
  }*/

  p_format = 0;
  format_is_up_to_date  = false;
  type = c->type;
}
/*
void Polygon::get_points(Point* p, int last_p)
{
  p.size() = last_p;
  p = (Point*)malloc(sizeof(Point) * p.size());
}
*/
Polygon::Polygon(const Polygon& a)
{
  exit(0);
  p = a.p;
}

Shape* Polygon::copy()
{
  Polygon *a;
  a  = new Polygon();

  // a->p.size() = p.size();
  a->format_is_up_to_date = false;

  // a->p = (Point*)malloc(sizeof(Point) * p.size());
  
  for(unsigned int i=0; i < p.size(); ++i) {
    a->p[i] = p[i];
  }
  
  return a;
}

/* EOF */
