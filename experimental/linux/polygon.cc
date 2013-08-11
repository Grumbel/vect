#include <stdio.h>
#include <allegro.h>
#include "objects.hh"

/* Polygon */
Polygon::Polygon()
{
  color = 255;
  p = NULL;
  last_point = 0;
}

void Polygon::create_new()
{
  int temp_x1, temp_y1;
  int temp_x2, temp_y2;
  int temp_x3, temp_y3;
  int mx, my;

  while(!mouse_button1_pressed());

  temp_x2 = mouse_x;
  temp_y2 = mouse_y;

  create_new_dot(temp_x2, temp_y2);
  
  temp_x1 = p[0].x;
  temp_y1 = p[0].y;
  temp_x3 = p[last_point - 1].x;
  temp_y3 = p[last_point - 1].y;

  show_mouse(NULL);
  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);
  
  while(!mouse_button2_pressed())
    {
      if (mx != mouse_x || my != mouse_y || mouse_b & 1)
	{
	  mx = mouse_x;
	  my = mouse_y;
    
	  line(screen,
	       p[0].x, p[0].y,
	       mx, my,
	       255);

	  line(screen,
	       temp_x1, temp_y1,
	       temp_x2, temp_y2,
	       255);
		  
	  if (last_point > 1)
	   	    {
	      line(screen,
		   p[last_point - 1].x, p[last_point - 1].y,
		   mx, my,
		   255);
	      
	      line(screen,
		   temp_x3, temp_y3,
		   temp_x2, temp_y2,	   
		   255);
	    }
	  
	  temp_x1 = p[0].x;
	  temp_y1 = p[0].y;
	  temp_x2 = mx;
	  temp_y2 = my;
	  temp_x3 = p[last_point - 1].x;
	  temp_y3 = p[last_point - 1].y;
    
	  if (mouse_button1_pressed())
	    {
	      if (last_point > 1)
		{
		  line(screen,
		       p[last_point - 1].x, p[last_point - 1].y,
		       mx, my,
		       255);
		 }
	      create_new_dot(mx, my);
	    }
	}
    }
  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
  show_mouse(screen);
}

void Polygon::move(int x, int y)
{
  for(int i=0; i<last_point; ++i) {
    p[i].x += x;
    p[i].y += y;
  } 
}

void Polygon::create_new_dot(int x, int y)
{
  p = (Point*)realloc(p, (2+last_point)*sizeof(*p));
  p[last_point].x = x;
  p[last_point].y = y;
  ++last_point;
}

void Polygon::draw_rect(BITMAP *scr)
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
  
  rect(scr, x1-1, y1-1, x2+1, y2+1, 255);
  
  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
}

void Polygon::draw(BITMAP *scr)
{
  int *a;
  a = (int*)malloc(sizeof(*a)* (last_point*2));
  
  for(int i=0; i<last_point; i+=1) {
    a[2*i] = p[i].x;
    a[2*i+1] = p[i].y;
  }
  
  polygon(scr, last_point, a, color);
}

void Polygon::save(FILE *out)
{
  fprintf(out, "[Polygon]\n");
  fprintf(out, "Color: %d\n", color);
  
  for(int i=0; i<last_point; ++i) {
    fprintf(out, "%5d %5d\n", p[i].x, p[i].y);
  }
  fprintf(out, "\n");
}

void Polygon::load(FILE *in)
{
  int d=1;
  char buf[256];

  p = (Point *)malloc(sizeof(Point) * 15);

  fgets(buf, 255, in);
  
  if (sscanf(buf, "Color: %d", &color) != 1) {
    fprintf(stderr, "Fehler beim laden von Polygon\n");
    puts(buf);
    exit(1);
  }

  for(last_point  = 0; ; ++last_point)
    {
      fgets(buf, 255, in);
      //    puts(buf);
      
      if (sscanf(buf, "%d %d", &p[last_point].x, &p[last_point].y) != 2)
	break;
	
      if (last_point > 14*d)
	{
	  p = (Point *)realloc(p, sizeof(Point) * 15 * d);
	  ++d;
	}
    }
  
  // puts("Polygon geladen");
}


