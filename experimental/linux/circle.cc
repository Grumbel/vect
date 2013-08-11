#include <stdio.h>
#include <allegro.h>
#include "objects.hh"

Circle::Circle()
{
  color = 255;
}

Circle::~Circle()
{
}

void Circle::move(int xm, int ym)
{
 x += xm;
 y += ym;
}

void Circle::draw_rect(BITMAP *scr)
{
  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);
  
  rect(scr, x-radius-1, y-radius-1, x+radius+1, y+radius+1, 255);
  
  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
}

void Circle::draw(BITMAP *scr)
{
  circle(screen, x, y, radius, color);  
}
void Circle::create_new()
{
  int count=0;
    
  int mx=0, mx1, my1;
  int temp_radius=0;
  int temp_x, temp_y;
  while (!mouse_button1_pressed());
  
  x = mouse_x;
  y = mouse_y;

  show_mouse(NULL);
  
  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);
  get_mouse_mickeys(&mx1, &my1);
  circle(screen, x, y, temp_radius, 255);
  
  while(!mouse_button1_pressed())
    {
      get_mouse_mickeys(&mx1, &my1);
      if (mx1 != 0)
	{	   
	  mx += mx1;
	  radius = abs(mx);
	  
	  circle(screen, x, y, radius, 255);
	  circle(screen, x, y, temp_radius, 255);
	  temp_radius = radius;
	}
    }
  
  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
  show_mouse(screen);
}

void Circle::save(FILE *out)
{
  fprintf(out, "[Circle]\n");
  fprintf(out, "Color: %d\n", color);
  fprintf(out, "Radius: %d\n", radius);
  fprintf(out, "%5d %5d\n", x, y);
  fprintf(out, "\n");
}

void Circle::load(FILE *in)
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
  //  fprintf(stderr, "Radius: %d\nX: %d Y: %d\n", r, x, y);
}

