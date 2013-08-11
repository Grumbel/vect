//       __     _         
//   |  /  | __| |  
//   | /  /_| _   > 
//   |/  / _|<_| |  
//   |__/___|__|_|  
//
//   A simple Vector-Graphic editor for Retriever.
//
//   Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>
//     
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <allegro.h>

#include "screen.hh"
#include "zoom.hh"
#include "add_aleg.hh"
#include "shapes.hh"
#include "status.hh"
#include "color.hh"
#include "menu.hh"
#include "defs.hh"
#include "unsorted.hh"
//Quick & Dirty !!!!!!!
#include "menudefs.cc"

#define DEBUG(x) x

char *menu_items[] =
  {
    "Create Polygon",
    "Create Circle",
    "Create Line",
    "Create Dots",
    "Create Text",
    "Create Rect",
    "???????",
    "???????",
    "--------------",
    "Files...    ->",
    "Help...     ->",
    "--------------",
    "Quit...     ->",
    NULL
  };

char *menu_help_items[] =
  {
    "Keybindings",
    "Help",
    "-----------",
    "About",
    "History",
    NULL
  };


char *menu_quit_items[] =
  {
     "Quit?"
    ,"-----"
    ,"Yes"
    ,"No"
    ,NULL
  };

char *menu_io_data[] =
  {
     "Load"
    ,"Save"
    ,"------------"
    ,"Load Bitmap"
    ,"Save Bitmap"
    ,"------------"
    ,"Load binary"
    ,"Save binary"
    ,"------------"
    ,"Load Palette"
    ,"Save palette"
    ,NULL
  };



Menu menu (menu_items);
Menu menu_io (menu_io_data);
Menu menu_help (menu_help_items);
Menu menu_quit (menu_quit_items);

/*
#define redraw lk
#define _redraw lk

void lk(vector<Shape*> a, int b){}
*/
//double zoom_f;
//int zoom_x, zoom_y;

BITMAP* background;
int   max_frames = 50;


////////////////////////
// |\  /|      o      //
// | \/ |  _      _   //
// |    | |_'| | | \| //
// |_||_| |_.| | ||\| //
////////////////////////
// The main function is a little bit to full!!!

char gfx_path[256] = {"graphics/"};

// Let the user interactively select color from a drawn palette
int get_current_color(int current_col) {
  PALETTE pal;
  Screen scr;
  scr.get();
  //  BITMAP *temp_scr;
  
  //backup_scr(&temp_scr);
  get_palette(pal);
  current_col = edit_palette(pal);
  //restore_scr(&temp_scr);
  scr.put();
  status_line.refresh();
  return current_col;
}

// Exit vect and print a goodby message
void exit_vect(int exit_code = EXIT_SUCCESS)
{
  allegro_exit ();
  std::cout <<
    "Vect V" << VECT_VERSION << "\n" 
    "~~~~~~~~~~~~~~~\n"
    "Thank you for using Vect, for more informations\n"
    "\n"
    "        http://home.pages.de/~grumbel/\n"
    "\n"
    "  Ingo Ruhnke <grumbel@gmx.de>\n"
       << std::endl;
  exit (EXIT_SUCCESS);
}
  

void update_status_line(int, int, int);

//Update the status line if necessary
void
update_status_line(int current_col, int current_obj, int last_obj)
{
  static int  mx, my;
  char str[256];
  // if the values have chaged update the status line
  if (mx != mouse_x || mouse_y != my) {
    sprintf (str,
	     "X: %4d Y: %4d Color: %3d Shape: %3d All: %3d",
	     mouse_x, mouse_y, current_col, current_obj, last_obj);
    
    show_mouse (NULL);
    status_line.print (str);
    show_mouse (screen);
    
    mx = mouse_x;
    my = mouse_y;
  }
}

// global variables
std::vector<std::vector<Shape*> > frames(max_frames);
std::vector<int> last_objs(max_frames);

//void menu_buttons(void))[10];
void
void_button(void){
}

void mouse_clear_buttons()
{
  while(mouse_b);
}

void mouse_wait_for_button()
{
  while(!mouse_b);
  mouse_clear_buttons();
}

// unfinished, it is just a dirty hack
// generate a nice 4/3 rectangle from the given one
void
make_4_to_3(int* x1, int* y1, int* x2, int* y2)
{
  *y2 = int(*y1 + ((*x2 - *x1) * (3/4.0)));
}

// Let the user interactively select a region to zoom in
void
interactive_zoom(void) {
  status_line.print("Interactive Zoom");
  
  mouse_clear_buttons();
  mouse_wait_for_button();

  int x1, x2, y1, y2;
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
    std::swap(x1, x2);
  if (y1 > y2)
    std::swap(y1, y2);
  
  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

  make_4_to_3(&x1, &y1, &x2, &y2);

  rect(screen, x1, y1, x2, y2, LINE_COLOR);

  Zoom::set_zoom(x2 - x1);
  Zoom::set_zoom_x(-x1);
  Zoom::set_zoom_y(-y1);
  //  redraw (shapes, last_obj);  
}

// don't ask me why that works!? %-)
typedef void (*Menu_Buttons)(void);

Menu_Buttons menu_buttons[] = {
  interactive_zoom,
  void_button,
  void_button,
  void_button,
  void_button,
  void_button
};

std::vector<Shape*> shapes;  // The main object which hold all sub objs
//Group shapes;

std::vector<Shape*> caption; // This obj holds all objects of a caption
//Group caption;

// this function is _very_ full
int
main (int argc, char *argv[])
{
  char    filename[256] = "";
  int     current_col = 255;
  int     c;
  int     scr_w = 640, scr_h = 480;
  int     current_obj = -1;
  int     last_obj = 0;
  int     current_frame = 0;
  int     mouse_buttons;

  //zoom_f = 1;
  //zoom_x = zoom_y = 0;

  /* Animation
     for(unsigned int i=0; i < frames.size(); ++i) {
     frames[i].resize(256);
     last_objs[i] = 0;
     }*/
  shapes.resize(255);
  // shapes = frames[0];
  
  opterr = 0;
  while ((c = getopt (argc, argv, "w:h:f:p:v")) != -1) {
    switch (c) {
    case 'w':
      scr_w = atoi (optarg);
      break;
    case 'h':
      scr_h = atoi (optarg);
      break;
    case 'f':
      strcpy(filename, optarg);
      break;
    case 'p':
      strcpy(gfx_path, optarg);
      break;
    case 'v':
      std::cout << "Vect V" << VECT_VERSION << std::endl;
      exit(EXIT_SUCCESS);
      break;
    case '?':
      std::cout <<
	"\n" 
	"Usage: VECT.EXE [OPTIONS]\n" 
	"~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	"-p PATH    Set the default path for load/save\n" 
	"-f FILE    Open FILE, for editing (buggy)\n" 
	"-h XXX     Use screenmode with height XXX\n" 
	"-w XXX     Use screenmode with width XXX\n"
	"-v         Print Version\n"
	   << std::endl;
      exit (EXIT_SUCCESS);
    }
  }

  // Init all Allegro stuff (keyboard, mouse, timer, ...)
  allegro_init ();
  install_keyboard ();
  install_timer ();

  if ((mouse_buttons = install_mouse() ) == -1) {
    std::cerr << "No mouse found!" << std::endl;
    exit (EXIT_FAILURE);
  } else {
    std::cout << mouse_buttons << "-button mouse found" << std::endl;
  }

  std::cout << "Trying to init graphics" << std::endl;
  if (set_gfx_mode (GFX_AUTODETECT, scr_w, scr_h, 0, 0) < 0) {
    std::cerr << allegro_error << std::endl;
    exit (EXIT_FAILURE);
  }
  
  // set gray-scale palette, with the first colors fixed for the menues
  if (filename[0] == '\0') {
    init_palette ();
  } else {
    last_obj = load_shape (shapes, last_obj, filename);
    redraw(shapes, last_obj);
  }
  show_mouse (screen); 
  
  // Main programm loop while not 'q' pressed
  // or Quit->Yes is selected over the menu
  // continue in the edit mode.
  while ((c >> 8) != KEY_Q) {
    while (!keypressed ()) {
      update_status_line(current_col, current_obj, last_obj);

      // display buttons
      if (mouse_y < 2)
	menu_buttons[display_buttons()]();
           
      // gets the current Shape, after a button1 press
      if (mouse_button1_pressed ()) {
	BITMAP *col_scr;
	int temp_obj = current_obj;

	col_scr = create_bitmap (SCREEN_W, SCREEN_H);
	clear (col_scr);
	_redraw_col (col_scr, shapes, last_obj);
	current_obj = getpixel (col_scr, mouse_x, mouse_y) - 1;

	// move the object, if it is marked
	if (current_obj == temp_obj && current_obj != -1) {
	  int x, y_move = 0;
	  int y, x_move = 0;
	  char str[250];

	  show_mouse (NULL);
	  get_mouse_mickeys (&x, &y);

	  while (mouse_b & 1) {
	    do {
	      get_mouse_mickeys (&x, &y);
	    }
	    while ((mouse_b & 1) && x == 0 && y == 0);
	    
	    shapes[current_obj]->move (x, y);
	    x_move += x;
	    y_move += y;

	    _redraw (shapes, last_obj);
	    sprintf (str, "Verschiebe Objekt %d %d", x_move, y_move);
	    status_line.print (str);
	  }
	  
	  //show_mouse (screen);
	} else if (current_obj == -1) {
	  // open group rectangel
	  int mx = mouse_x, my = mouse_y;
	  int x = mx, y = my;
	  int temp_mx = mx, temp_my = my;
	  
	  drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);
	  rect(screen, x, y, mx, my, LINE_COLOR);
	  
	  while(mouse_b & 1) {
	    if (mouse_x != mx || mouse_y != my) {
	      mx = mouse_x;
	      my = mouse_y;
	      
	      show_mouse(NULL);
	      rect(screen, x, y, temp_mx, temp_my, LINE_COLOR);
	      rect(screen, x, y, mx, my, LINE_COLOR);
	      show_mouse(screen);
	      
	      temp_mx = mx;
	      temp_my = my;
	    }
	  }
	  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

	  caption.clear();
	  
	  for(int i = 0; i < last_obj; ++i) {
	    if (shapes[i]->is_in(x, y, mx, my)) {
	      caption.push_back(shapes[i]);
	      status_line.print("Have caption");
	    }
	  }

	  rect(screen, x, y, mx, my, LINE_COLOR);
	  
	  for(unsigned i = 0; i < caption.size(); ++i) {
	    caption[i]->draw_rect(screen);
	  }

	  // caption.draw_rect(screen);

  	} else { //_---
	  show_mouse (NULL);
	
	  char str[255];
	  sprintf (str, "Current Shape: %5d", current_obj);
	  status_line.print (str);
	  
	  if (current_obj >= last_obj || current_obj < 0)
	    current_obj = -1;
	  
	  _redraw (shapes, last_obj);
	  if (current_obj != -1)
	    shapes[current_obj]->draw_rect (screen);
	  show_mouse (NULL);
	}
	destroy_bitmap (col_scr);
	status_line.current_obj = current_obj;
	status_line.last_obj = last_obj;
      }
      // show the palette and gets the current color
      if (mouse_button3_pressed ()) {
	current_col = get_current_color(current_col);
      }

      // if button2 is pressed show menu and do the stuff
      if (mouse_button2_pressed ()) {
	status_line.current_obj = current_obj;
	status_line.last_obj = last_obj;

	switch (menu.show ()) {
	case 0:		// Polygon
	  status_line.print("Create new Polygon");
	  shapes[last_obj] = new Polygon (current_col);
	  current_obj = last_obj;
	  shapes[last_obj++]->create_new();
	  break;

	case 1:		// Circle
	  status_line.print("Create new Circle");

	  shapes[last_obj] = new Circle(current_col);
	  shapes[last_obj++]->create_new ();
	  break;

	case 2:		// Line
	  status_line.print("Create new Line");

	  shapes[last_obj] = new Line(current_col);
	  shapes[last_obj++]->create_new ();
	  break;

	case 3:               // Dots
	  status_line.print("Create new Dots");

	  shapes[last_obj] = new Dot(current_col);
	  shapes[last_obj++]->create_new();
	  break;
	    
	case 4:               // Text
	  status_line.print("Create new Text");

	  shapes[last_obj] = new Text(current_col);
	  shapes[last_obj++]->create_new();
	  break;

	case 5:
	  status_line.print("Create new Rect");
	    
	  shapes[last_obj] = new Rect(current_col);
	  shapes[last_obj++]->create_new();
	  break;

	case 6:
	  break;

	case 7:
	  break;
	    
	case 9:		// Files...

	  switch (menu_io.show ()) {
	  case 0:		// Load
	    last_obj = load_file(shapes, last_obj);
		
	    if (last_obj > 0) {
	      clear(screen);
	      redraw(shapes, last_obj);
	    }
	    break;

	  case 1:		// Save
	    // Temporary remmed for linux
	    // menu_save_rgf();
	    // End
	    break;

	  case 3:
	    {	// Load Bitmap
	      char filename[256];
	      PALETTE pal;
		  
	      strcpy(filename, gfx_path);

	      if (file_select ("Load Bitmap", filename, NULL)) {
		background = load_bitmap (filename, pal);
	      }
	      break;
	    }
	  case 4: // Save Bitmap
	    {
	      BITMAP *tmp_scr;
	      PALETTE pal;
	      char filename[256];

	      strcpy(filename, gfx_path);
	      backup_scr(&tmp_scr);
	      get_palette (pal);
		  
	      if (file_select ("Save Bitmap", filename, NULL)) {
		if (!exists(filename)) {
		  save_pcx (filename, tmp_scr, pal);
		}
	      }
	      restore_scr(&tmp_scr);
	      break;
	    }
	  case 6: // Load binary
	    draw_textbox(screen, mouse_x, mouse_y,
			 "The function to load the file in "
			 "binary format is not implementated "
			 "yet, sorry.");
	    break;
	  case 7: // Save binary
	    draw_textbox(screen, mouse_x, mouse_y,
			 "The function \nto save the file in "
			 "binary format\n is not implementatet "
			 "yet, sorry.");
	    break;
	  case 9: // Load Palette
	    {
	      char filename[256];
	      strcpy(filename, gfx_path);
		  
	      if (file_select ("Load Palette", filename, NULL)) {
		FILE *in;

		in = fopen(filename, "rt");

		if (in != NULL) {
		  fscanf(in, "[Palette]\n");
		  load_palette(in);
		  fclose(in);
		}
	      }
	    }
	    break;
	  case 10: // Save Palette
	    {
	      char filename[256];
	      strcpy(filename, gfx_path);
	      if (file_select ("Save Palette", filename, NULL)) {
		if (!exists(filename)) {
		  FILE *out;

		  out = fopen(filename, "wt");

		  if (out != NULL) {
		    save_palette(out);
		    fclose(out);
		  }
		}
	      }
	    }
	    break;
	  }
	  //  switch (menu_io.show())
	  break;
	case 10:		// Help...
	    
	  switch (menu_help.show ()) {
	  case 0:           // Keys
	    display_text ("keys.txt");
	    break;
	      
	  case 1:		// Help
	    display_text("help.txt");
	    break;

	  case 3:		// About
	    display_text("about.txt");
	    break;

	  case 4:
	    display_text("history.txt");
	    break;
	  }
	  break;

	  case 12:		// Quit...

	  switch (menu_quit.show ()) {
	  case 2:		// Yes
	    exit_vect();
	    break;
	  case 3:		// No
	    break;
	  }
	  break;
	}
	// switch (menu.show())
	show_mouse (NULL);
	_redraw (shapes, last_obj);
	show_mouse (screen);
      } // if (mouse_button2_pressed())
    } // while(!keypressed())

    c = readkey ();

    DEBUG(std::cerr << "Key: " << c << " pressed.");

    switch (c >> 8) {
    case KEY_D:
      redraw (shapes, last_obj);
      break;

    case KEY_SPACE:
      show_mouse(NULL);
      clear(screen);		//      switch (menu.show())
      show_mouse(screen);
      break;

    case KEY_B:
      if (background != NULL) {
	clear (screen);
	stretch_blit (background, screen,
		      0, 0, background->w, background->h,
		      0, 0, SCREEN_W, SCREEN_H);
	redraw (shapes, last_obj);
      }
      break;

    case KEY_TAB:
      if (last_obj > 0) {
	if (!(key_shifts & KB_SHIFT_FLAG)) {
	  if (++current_obj >= last_obj)
	    current_obj = 0;
	} else {
	  if (--current_obj < 0)
	    current_obj = last_obj - 1;
	}
	show_mouse (NULL); 

	_redraw (shapes, last_obj);

	shapes[current_obj]->draw_rect (screen);
	show_mouse (screen);
      }
      break;

    case KEY_PGUP:
      if (key_shifts & KB_SHIFT_FLAG) {
	current_obj = raise_obj(shapes, last_obj, current_obj, last_obj - 1);
      } else {
	current_obj = raise_obj(shapes, last_obj, current_obj, 1);
      }
      break;

    case KEY_PGDN:
      if (key_shifts & KB_SHIFT_FLAG) {
	current_obj = raise_obj(shapes, last_obj, current_obj, -current_obj);
      } else {
	current_obj = raise_obj(shapes, last_obj, current_obj, -1);
      }
      break;

    case KEY_U:
      {
	PALETTE pal;

	get_palette (pal);
	edit_palette (pal);
      }
      break;

    case KEY_C:
      if (current_obj != -1) {
	PALETTE pal;
	shapes[current_obj]->set_color(edit_palette(pal));
	redraw(shapes, last_obj);
      }
      break;

    case KEY_P:
      {
	int c1 = 0;
	int c2 = 0;

	PALETTE pal;
	get_palette (pal);
	c1 = edit_palette(pal);
	c2 = edit_palette(pal);
	fade_colors (pal, c1, c2);
	set_palette(pal);
	break;
      }
      
    case KEY_V:
      {
	if (current_obj != -1) {
	  shapes[current_obj]->draw_vertex ();

	  while(key[KEY_V]);
	  int temp_mouse_x = mouse_x;
	  int temp_mouse_y = mouse_y;
	    
	  while(!key[KEY_V]) {
	    if (key[KEY_DEL]) {
	      shapes[current_obj]->del_vertex();
	    }
	      
	    if (mouse_x != temp_mouse_x
		&& mouse_y != temp_mouse_y)
	      {
		shapes[current_obj]->draw_vertex();
		shapes[current_obj]->show_vertex();
		temp_mouse_x = mouse_x;
		temp_mouse_y = mouse_y;
	      }
	    if (mouse_b & 1) {
	      shapes[current_obj]->move_vertex();
	    }
	  }
	  while(key[KEY_V]);
	  clear_keybuf();
	}
      }
      break;
	
    case KEY_DEL:
      if (current_obj != -1) {
	delete shapes[current_obj];

	for(int i=current_obj; i < last_obj - 1; ++i) {
	  shapes[i] = shapes[i + 1];
	}
	current_obj = -1;
	--last_obj;
	redraw(shapes, last_obj);
      }
      break;

    case KEY_S:
      if (current_obj != -1) 
	shapes[current_obj]->scale();
      break;

    case KEY_R:
      if (current_obj != -1) 
	shapes[current_obj]->rotate();
      break;
	
    case KEY_PLUS_PAD:
      if (current_obj != -1)
	shapes[last_obj++] = shapes[current_obj]->copy();
      break;

    case KEY_G:
      if (caption.size() > 1) {
	shapes[last_obj++] = new Group(caption);
	for (unsigned int z=0; z < caption.size(); ++z) {
	  for (unsigned int i=0; i < shapes.size(); ++i) {
	    if (shapes[i] == caption[z]) {
	      shapes.erase(shapes.begin() + i);
	      --last_obj;
	    }
	  }
	}
	caption.clear();
      }

      // clog << current_obj << std::endl;
	
      if (current_obj != -1 && shapes[current_obj]->type == GROUP) {
        std::cout << "Ungroup" << std::endl;
	Group* b;
	b = static_cast<Group*>(shapes[current_obj]);

	for(unsigned int i = 0; i < b->size(); ++i) {
	  shapes.insert(shapes.begin() + current_obj, b->at(b->size()-i-1));
	  ++last_obj;
	}
	--last_obj;
	delete b;
      }
	
      break;

    case KEY_Y: // zoom in
      Zoom::zoom_f *= 1.1;
      Zoom::zoom_x = (int)((-(int)((Zoom::zoom_f - 1.0) * (SCREEN_W/2.0))) / Zoom::zoom_f);
      Zoom::zoom_y = (int)((-(int)((Zoom::zoom_f - 1.0) * (SCREEN_H/2.0))) / Zoom::zoom_f);
      redraw(shapes, last_obj);
      break;

    case KEY_T: // zoom out
      Zoom::zoom_f /= 1.1;
      Zoom::zoom_x = (int)((-(int)((Zoom::zoom_f - 1.0) * (SCREEN_W/2.0))) / Zoom::zoom_f);
      Zoom::zoom_y = (int)((-(int)((Zoom::zoom_f - 1.0) * (SCREEN_H/2.0))) / Zoom::zoom_f);
      redraw(shapes, last_obj);	
      break;

    case KEY_LEFT:
      Zoom::zoom_x += 20;
      redraw(shapes, last_obj);	
      break;
    case KEY_RIGHT:
      Zoom::zoom_x -= 20;
      redraw(shapes, last_obj);	
      break;
    case KEY_UP:
      Zoom::zoom_y += 20;
      redraw(shapes, last_obj);	
      break;
    case KEY_DOWN:
      Zoom::zoom_y -= 20;
      redraw(shapes, last_obj);	
      break;

    case KEY_N:
      frames[current_frame] = shapes;
      last_objs[current_frame] = last_obj;

      ++current_frame;
      if (current_frame > max_frames)
	current_frame = max_frames;
	
      shapes = frames[current_frame];
      last_obj = last_objs[current_frame];

      redraw(shapes, last_obj);
      break;

    case KEY_M:
      frames[current_frame] = shapes;
      last_objs[current_frame] = last_obj;

      --current_frame;
      if (current_frame < 0)
	current_frame = 0;

      shapes = frames[current_frame];
      last_obj = last_objs[current_frame];

      redraw(shapes, last_obj);
      break;
	
    default:
      break;
    }
  }

  exit_vect();
  return 0;
}
 
/* EOF */
