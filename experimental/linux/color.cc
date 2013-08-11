#include <stdlib.h>
#include <allegro.h>

#include "add_aleg.hh"
#include "status.hh"

extern Status_Line status_line;

void  edit_palette(PALETTE pal)
{
  char str[256];
  int index=0;
  int x1, y1;
  int mx, my;
  int temp_index;
  
  x1 = SCREEN_W / 2 - 136;
  y1 = SCREEN_H / 2 - 136;

  show_mouse(NULL);

  rectfill(screen, x1-3, y1-3, x1 + 273, y1 + 273, 0);
  rect(screen, x1-3, y1-3, x1 + 273, y1 + 273, 255);
  
  for(int y = y1; y < y1 + 272; y += 17)
    for(int x = x1; x < x1 + 272; x += 17)
      rectfill(screen, x, y, x+15, y+15, index++);

  show_mouse(screen);

  while(!mouse_button1_pressed())
    {
      mx = mouse_x;
      my = mouse_y;
      
      mx -= x1;
      my -= y1;
      
      mx /= 17;
      my /= 17;

      index = my * 16 + mx;
      if (index > 255 || index < 0)
	index = temp_index;
      
      if (index != temp_index)
	{
	  itoa(index, str, 10);
	  show_mouse(NULL);
	  status_line.print(str);
	  temp_index = index;
	  show_mouse(screen);
	}
    }
}
