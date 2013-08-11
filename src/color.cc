// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <string.h>

#include "add_aleg.hh"
#include "status.hh"
#include "defs.hh"
#include "color.hh"

//int _mangled_main_address;

extern Status_Line status_line;

// An empty function, it is a place holder
// for the dialog. 
int
update_color(void *dp3, int val){return 0;}

// Displays the palette, you can then select
// a color which index is then returned.
void draw_palette(PALETTE pal)
{
  int box_width = 8;
  int pal_width = 16 * (box_width + 1);
  int index=0;
  int x1, y1;
 
  status_line.print("Select Color");
  
  x1 = mouse_x - pal_width/2;
  y1 = mouse_y - pal_width/2;
  /*
  if (x1 + pal_width > SCREEN_W)
    x1 = SCREEN_W - pal_width -3;

  if (x1 < 0)
    x1 = 3;

  if (y1 < 0)
    y1 = 3;
  
  if (y1 + pal_width > SCREEN_H - 15)
    y1 = SCREEN_H - pal_width -15;
*/
  show_mouse(NULL);

  /*
    rectfill(screen,
	   x1 - 3,
	   y1 - 3,
	   x1 + pal_width + 1,
	   y1 + pal_width + 1,
	   0);
   */
  
  rect(screen,
       x1 - 3,
       y1 - 3,
       x1 + pal_width + 1,
       y1 + pal_width + 1,
       255);
  
  for(int y = y1; y < y1 + pal_width; y += box_width + 1)
    for(int x = x1; x < x1 + pal_width; x += box_width + 1)
      rectfill(screen, x, y, x + box_width - 1, y + box_width - 1, index++);

  show_mouse(screen);
}

int
edit_palette(PALETTE pal)
{
  char str[256];
  int mx, my;
  int x1, y1;
  int box_width = 8;
  int pal_width = 16 * (box_width + 1);
  int index=0, temp_index = 0;
  
  x1 = mouse_x - pal_width/2;
  y1 = mouse_y - pal_width/2;

  if (x1 + pal_width > SCREEN_W)
    x1 = SCREEN_W - pal_width -3;

  if (y1 + pal_width > SCREEN_H)
    y1 = SCREEN_H - pal_width -3;
  
  draw_palette(pal);

  while(mouse_b);

  while(!mouse_b)
    {
      mx = mouse_x;
      my = mouse_y;
      
      mx -= x1;
      my -= y1;
      
      mx /= box_width + 1;
      my /= box_width + 1;

      index = my * 16 + mx;

      if (mx < 0 || mx > 15 || my < 0 || my > 15)
	index = temp_index;
      
      if (index != temp_index)
	{
	  sprintf(str, "%d", index);
	  show_mouse(NULL);
	  status_line.print(str);
	  temp_index = index;
	  show_mouse(screen);
	}
    }

  if (mouse_button2_pressed()) {
    RGB color;
    
    color = change_color();
    set_color(index, &color);
    get_palette(pal);
    
    return index;
  }

  if (mouse_button1_pressed()
      || mouse_button3_pressed()
      || mouse_b & 1
      || mouse_b & 4)
    {
      char str[256];
      sprintf(str, "Current Color: %d", index);
      status_line.print(str);
      return index;
    }
  return index;
}
 
// Displays a dialog menu, were you can change
// a palette color, at the moment this implementation
// is very lausy.
RGB
change_color(void)
{
  BITMAP *temp_scr;
  RGB color;
  
  DIALOG select_color[] = {
    //(dialogproc)  (x)  (y)   (w) (h)
    // (fg)(bg)(key)(flags)(d1)(d2)(dp)(dp2)(dp3)
    { d_slider_proc,  32,  32, 256, 16,
      TEXT_COLOR   ,  TEXT_BG_COLOR,  0,  0, 63,   0, NULL, update_color, 0 },
    { d_slider_proc,  32,  64, 256, 16,
      TEXT_COLOR   , TEXT_BG_COLOR,0,   0,  63,  0, NULL, update_color, 0 },
    { d_slider_proc,  32,  96, 256, 16, 
      TEXT_COLOR   , TEXT_BG_COLOR,0,   0,  63,  0, NULL, update_color, 0 },
    { d_text_proc  , 300,  38,   0, 0,
      TEXT_COLOR,    TEXT_BG_COLOR,   0,   0,  0,    0, (void*)"R"  },
    { d_text_proc  , 300,  70,   0,  0,
      TEXT_COLOR   , TEXT_BG_COLOR,   0,   0,  0,    0, (void*)"G"  },
    { d_text_proc  , 300, 102,   0,  0,
      TEXT_COLOR   , TEXT_BG_COLOR,   0,   0,  0,    0, (void*)"B"  },
    { NULL,0,0,0,0,0,0,0,0,0,0,NULL}
  };

  status_line.print("Edit Color");
  backup_scr(&temp_scr);
  //get_palette(pal);
  
  //  set_palette(desktop_palette);
  do_dialog(select_color, -1);  

  color.r = select_color[0].d2;
  color.g = select_color[1].d2;
  color.b = select_color[2].d2;

  //set_palette(pal);

  restore_scr(&temp_scr);

  return color;
}

// Init a gray scale palette with the first 32 colors
// fixed for the menus and text colors.
void
init_palette(void)
{
  char* vectrc = "default.pal";
  FILE* in = fopen(vectrc, "rt");

  if (in == NULL) {
    allegro_exit();
    std::cout << "Could not find default palette \"" << vectrc << "\"" << std::endl;
    exit(EXIT_FAILURE);
  }

  fscanf(in, "[Palette]\n");
  load_palette(in);
  fclose(in);
  
    /*  
  for(int i=0; i<256; ++i) 
    pal[i].r = pal[i].b = pal[i].g = i/4;

  pal[TEXT_COLOR].r = 63;
  pal[TEXT_COLOR].g = 63;
  pal[TEXT_COLOR].b = 63;

  pal[TEXT_BG_COLOR].r = 0;
  pal[TEXT_BG_COLOR].g = 0;
  pal[TEXT_BG_COLOR].b = 0;

  pal[MENU_BOX_COLOR].r = 32;
  pal[MENU_BOX_COLOR].g = 32;
  pal[MENU_BOX_COLOR].b = 32;
  
  pal[MENU_DBOX_COLOR].r = 15;
  pal[MENU_DBOX_COLOR].g = 15;
  pal[MENU_DBOX_COLOR].b = 15;
  
  pal[MENU_LBOX_COLOR].r = 63;
  pal[MENU_LBOX_COLOR].g = 63;
  pal[MENU_LBOX_COLOR].b = 63;

  pal[MENU_TEXT_COLOR].r = 63;
  pal[MENU_TEXT_COLOR].g = 63;
  pal[MENU_TEXT_COLOR].b = 63;
  
  pal[MENU_HTEXT_COLOR].r = 0;
  pal[MENU_HTEXT_COLOR].g = 0;
  pal[MENU_HTEXT_COLOR].b = 0;

  pal[VERTEX_COLOR].r = 0;
  pal[VERTEX_COLOR].g = 63;
  pal[VERTEX_COLOR].b = 0;

  pal[LINE_COLOR].r = 63;
  pal[LINE_COLOR].g = 63;
  pal[LINE_COLOR].b = 63;
  
  pal[255].r = 0;
  pal[255].g = 63;
  pal[255].b = 0;
  set_palette(pal);*/
}

void
load_palette(FILE *in)
{
  PALETTE pal;
  int r,g,b;
  
  get_palette(pal);

  for(int i=0; i < 256; ++i) {
    fscanf(in, "%d %d %d\n", &r, &g, &b);

    pal[i].r = r;
    pal[i].g = g;
    pal[i].b = b;
  }

  fscanf(in, "\n");
  
  set_palette(pal);
}

void
save_palette(FILE *out)
{
  PALETTE pal;
  get_palette(pal);

  fprintf(out, "[Palette]\n");
  
  for(int i=0; i < 256; ++i)
    fprintf(out, "%d %d %d\n", pal[i].r, pal[i].g, pal[i].b);
  
  fputc('\n', out);
}

void
fade_colors(PALETTE pal, int c1, int c2)
{
  int range = c2 - c1;
  int div_r = pal[c2].r - pal[c1].r;
  int div_g = pal[c2].g - pal[c1].g;
  int div_b = pal[c2].b - pal[c1].b;
  double steps_r = (double)div_r / range;
  double steps_g = (double)div_g / range;
  double steps_b = (double)div_b / range;
  
  fprintf(stderr, "%3d %3d %3d\n", pal[c1].r, pal[c1].b, pal[c1].g);

  for(int i = c1 + 1; i < c2; ++i) {  
    pal[i].r = (unsigned char)(pal[c1].r + ((i-c1) * steps_r)); 
    pal[i].g = (unsigned char)(pal[c1].g + ((i-c1) * steps_g)); 
    pal[i].b = (unsigned char)(pal[c1].b + ((i-c1) * steps_b)); 

    fprintf(stderr, "%3d %3d %3d\n", pal[i].r, pal[i].b, pal[i].g);
  }

  fprintf(stderr, "%3d %3d %3d\n", pal[c2].r, pal[c2].b, pal[c2].g);
}

/* EOF */
