// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <allegro.h>

#include "add_aleg.hh"
#include "shapes.hh"
#include "color.hh"
#include "unsorted.hh"
#include "defs.hh"

extern char gfx_path[256];
extern BITMAP* background;
extern Status_Line status_line;

void istream_skip(std::istream& in, const char* str)
{
  for(int i = 0; str[i] != '\0' && in.good(); ++i)
  {
    if (in.get() != str[i])
    {
      in.setstate(std::ios::failbit);
      std::cerr << "error: failured to skip: " << str << std::endl;
      return;
    }
  }
}

// no longer in use
// check if all objects have a defined value, exit if an undefined
// value occure
void check_obj(std::vector<Shape*>a, int last_obj)
{
  //cerr << "Check: Checking Shapes" << std::endl;
  
  for(int i=0; i < last_obj; ++i) {
    switch(a[i]->type) {
      case LINE:
      case CIRCLE:
      case POLYGON:
	break;
      case SHAPE:
        std::cerr << "Check: Shape type \"Shape\", bailout" << std::endl;
	exit(EXIT_FAILURE);
	break;
	
      default:
	std::cerr << "Check: Unkown Shape type!" << (int)a[i]->type << std::endl;
	exit(EXIT_FAILURE);
	break;
    }
  }
  //std::cerr << "Check: Shapes OK." << std::endl;
}


// Raise or lower the object to the given position
int
raise_obj(std::vector<Shape*>&a, int last_obj, int current_obj, int pos)
{
  Shape* temp;
  int next_obj = current_obj + pos;

  if (next_obj >= last_obj)
    next_obj = last_obj - 1;
  else if (next_obj < 0)
    next_obj = 0;
  
  if (current_obj == next_obj) {
    return current_obj;
  } else {
    temp = a[current_obj];

    if (next_obj > current_obj) {
      for(int i = current_obj; i < next_obj; ++i) {
	a[i] = a[i + 1];
      }

      a[next_obj] = temp;
    } else {
      for(int i = current_obj; i > next_obj; --i) {
	a[i] = a[i - 1];
      }
      a[next_obj] = temp;
    }

    current_obj    = next_obj;
    clear(screen);
    redraw(a, last_obj);	      
    a[current_obj]->draw_rect(screen);

    return current_obj;
  }
  
  /*
    if (++current_obj >= last_obj) {
    current_obj = last_obj - 1;
  } else {
    clear(screen);
    redraw(a, last_obj);	      
    a[current_obj]->draw_rect(screen);
  }
  */
}

// Loads a file in a char** array
int
get_txt_file(char *filename, char **str)
{
  FILE *in;
  char buffer[105];
  int line;

  in = fopen(filename, "rt");

  if (in == NULL) {
    perror (filename);
    return -1;
  }
  
  line = 0;
  str  = (char**)malloc(sizeof(char*));
  
  while(fgets(buffer,100,in)) {
    str = (char **)realloc(str, sizeof(char**) * (line + 1));
    str[line] = new char [strlen(buffer)+1];
    strcpy(str[line], buffer);
    ++line;
  }

  fclose(in);
  return line;
}

// Let the user interactivly choose a file to save the shapes in
void save_file(std::vector<Shape*>&a, int last_obj)
{
  FILE *out;
  char filename[256];

  strcpy(filename, gfx_path);
  
  BITMAP *temp_scr;

  backup_scr(&temp_scr);

  if (file_select_ex("Save", filename, NULL, 256, OLD_FILESEL_WIDTH, OLD_FILESEL_HEIGHT))
    {
      if ((exists(filename)
	   && _alert("File exist", get_filename(filename),
		     "overwrite",
		     "yes", "no", 'y', 'n') == 1)
	  || !exists(filename))
	{
	  out = fopen(filename, "wt");

	  save_palette(out);
	  
	  for(int i=0; i<last_obj; ++i)
	    a[i]->save(out);

	  fclose(out);
          std::cout << "Saveing: " << filename << "successfully saved" << std::endl;
      	}
    }

  restore_scr(&temp_scr);   
}

// Load the file to std::vector<Shape> without user interactions
int load_shape(std::vector<Shape*>&a, int last_obj, char* filename)
{
  FILE *in;
  char type[256];
  
  if (!(in = fopen(filename, "rt"))) {
    perror(filename);
    exit(EXIT_FAILURE);
  }
      
  for (int i = 0; i < last_obj; ++i)
    delete a[i];

  last_obj = 0;
      
  while(in != (FILE*)EOF) {
    //std::cerr << "Scan Line" << std::endl;
    
    if (fscanf(in, "[%[^]]]\n", type) != 1) {
      //std::cerr << "Can not scan!" << std::endl;
      break;
    }
    //std::cerr << "Line scanned" << std::endl;
      
    //std::cerr << "Shape: " << last_obj << " -> ";
	
    if (strcmp(type, "Circle") == 0) {
      a[last_obj] = new Circle;
      a[last_obj++]->load(in);
	
    } else if (strcmp(type, "Polygon") == 0) {
      a[last_obj] = new Polygon;
      a[last_obj++]->load(in);

    } else if (strcmp(type, "Line")  == 0) {
      a[last_obj] = new Line;
      a[last_obj++]->load(in);
	
    } else if (strcmp(type, "Palette") == 0) {
      load_palette(in);

    } else if (strcmp(type, "Rect") == 0) {
      a[last_obj] = new Rect;
      a[last_obj++]->load(in);

    } else if (strcmp(type, "Dot") == 0) {
      a[last_obj] = new Dot;
      a[last_obj++]->load(in);

    } else if (strcmp(type, "Text") == 0) {
      a[last_obj] = new Text;
      a[last_obj++]->load(in);
    } else if (strcmp(type, "Group") == 0) {
      a[last_obj] = new Group;
      a[last_obj++]->load(in);
    } else {
      std::cout << "Typ [" << type << "] unknown" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  fclose(in);

  return last_obj;
}

// Let the user interactively load an rgf file
int load_file(std::vector<Shape*>&a, int last_obj)
{
  char filename[256];

  strcpy(filename, gfx_path);

  BITMAP *temp_scr;

  backup_scr(&temp_scr);
  
  if (file_select_ex("Load", filename, "RGF", 256, OLD_FILESEL_WIDTH, OLD_FILESEL_HEIGHT)) {
    last_obj = load_shape(a, last_obj, filename);
    //    load_Shape(a, filename);
  }
  restore_scr(&temp_scr);

  return last_obj;
}


void load_Shape(std::vector<Shape*> &shapes, char* filename)
{
  PALETTE pal;
  std::ifstream in_file(filename);

  if (!in_file) {
    return;
  }
  
  istream_skip(in_file, "[Palette]\n");

  for(int i = 0; i < 256; ++i) {
    in_file >> pal[i].r >> pal[i].g >> pal[i].b;
    istream_skip(in_file, "\n");

    std::cout << "RGB: bluber "
	 << pal[i].r << "# "
	 << pal[i].g << "# "
	 << pal[i].b << std::endl;
  }
  
  set_palette(pal);

  in_file.close();
}

// ugly function to display a text file
void display_text (const char* filename)
{
  char *str[50];
  int lines = 0;
  
  FILE *in;

  in = fopen(filename, "rt");

  if (in == NULL) {
    allegro_exit();
    perror(filename);
    exit(EXIT_FAILURE);
  }

  while(true) {
    str[lines] = new char [100];

    if (!fgets(str[lines++],90, in))
      break;
  }
  
  fclose(in);
  /*
  int lines;
  char** str;
  lines = get_txt_file(filename, str);*/
  show_text (str, lines - 1);
}


// simple, nice, buggy textbox draw routine
void
draw_textbox(BITMAP *scr, int x_pos, int y_pos, const char* org_str)
{
  BITMAP *temp_scr;
  char str[50][50];
  
  while(mouse_b);
  backup_scr(&temp_scr);
  
  int height=0;
  int i;
  int x;
  int temp_x;
  int temp_i;
  int width = 0;
  
  for(i=0, x=0, temp_x=0, temp_i=0; org_str[i] != '\0'; ++i,++x)
    {
      if (x > 20)
	{
	  str[height][temp_x] = '\0';
	  x = 0;
	  i = temp_i + 1;
	  ++height;
	}

      if (org_str[i] == ' ')
	{
	  temp_i = i;
	  temp_x = x;
	}

      if (org_str[i] == '\n')
	{
	  str[height][x] = '\0';
	  ++height;
	    x=0;
	  ++i;
	  continue;
	}
      
      str[height][x] = org_str[i];
    }

  str[height][x] = '\0';

  for(int i=0; i <= height; ++i) {
    if ((signed int)strlen(str[i]) > width)
      width = strlen(str[i]);
  }
  show_mouse(NULL);
  rectfill(scr,
	   x_pos - 2 - (width*4), y_pos - 2 - ((height)*4),
	   x_pos +1+ (width * 4), y_pos + 1+ ((height+2) * 4), TEXT_BG_COLOR);

  rect(scr,
       x_pos - 2 - (width*4), y_pos - 2 - ((height)*4),
       x_pos + 1 + (width * 4), y_pos + 1 + ((height+2) * 4), TEXT_COLOR);
  
  for(int i=0; i <= height; ++i) {
    textout_ex(scr, font, str[i], x_pos-(width*4), y_pos - (height*4) +(i*8), TEXT_COLOR, 0);
  }
  while(!mouse_b && !keypressed());
  clear_keybuf();
  
  show_mouse(screen);
  restore_scr(&temp_scr);
}


// like redraw, but it draws the Shape with
// its position as color, used for the select
// routine
void
_redraw_col (BITMAP * scr, std::vector<Shape*>&a, int last_obj)
{
  for (int i = 0; i < last_obj; ++i)
    a[i]->draw_col (scr, i + 1);
  
  status_line.refresh ();
}

// simple function to draw a 3d looking rectangel,
// to build the graphicel base for the menu
void rect3d_up(BITMAP *scr, int x1, int y1, int x2, int y2)
{
  rectfill(scr, x1, y1, x2, y2, MENU_BOX_COLOR);

  line(scr, x1, y1, x1, y2, MENU_LBOX_COLOR);
  line(scr, x1, y1, x2, y1, MENU_LBOX_COLOR);

  line(scr, x2, y1, x2, y2, MENU_DBOX_COLOR);
  line(scr, x1, y2, x2, y2, MENU_DBOX_COLOR);
}


// simple function to draw a 3d looking rectangel,
// to build the graphicel base for the menu
void rect3d_down(BITMAP *scr, int x1, int y1, int x2, int y2)
{
  rectfill(scr, x1, y1, x2, y2, MENU_BOX_COLOR);

  line(scr, x1, y1, x1, y2, MENU_DBOX_COLOR);
  line(scr, x1, y1, x2, y1, MENU_DBOX_COLOR);

  line(scr, x2, y1, x2, y2, MENU_LBOX_COLOR);
  line(scr, x1, y2, x2, y2, MENU_LBOX_COLOR);
}


// simple function to draw a 3d looking rectangel,
// to build the graphicel base for the menu
void rect3d_n(BITMAP *scr, int x1, int y1, int x2, int y2)
{
  rectfill(scr, x1, y1, x2, y2, MENU_BOX_COLOR);

  //  line(scr, x1, y1, x1, y2, MENU_LBOX_COLOR);
  //line(scr, x1, y1, x2, y1, MENU_LBOX_COLOR);

  //line(scr, x2, y1, x2, y2, MENU_DBOX_COLOR);
  //line(scr, x1, y2, x2, y2, MENU_DBOX_COLOR);
}


int display_buttons()
{
  const int button_lenght = 50;
  BITMAP *temp_scr;
  int current_button = -1;
  int temp_button = -1;
  bool button_down=false;
  int pressed_button = 0;
  char str[] = "0\0";
  
  show_mouse(NULL);

  _backup_scr(&temp_scr);
  
  for (int x=0; x < 300; x += button_lenght)
    rect3d_n(screen, x, 1, x + 49, 29);

  show_mouse(screen);
  
  while (mouse_y < button_lenght) {
    for (int x=0; x < 6; ++x) {
      
      current_button = x;

      
      if (!(mouse_b & 1) && button_down) {
	restore_scr(&temp_scr);
	return x; 
      }
      
      if (mouse_x > (x * button_lenght) && mouse_x < (x*50) + 49 &&
	  temp_button != current_button ||
	  ((mouse_b & 1) && !button_down))
	{
	  show_mouse(NULL);

      str[0] = '0' + x;
      status_line.print(str);

	  
	  for (int x1=0; x1 < 300; x1+=50)
	    rect3d_n(screen, x1, 1, x1 + 49, 29);
	  
	  if (!(mouse_b & 1)) {
	    rect3d_up(screen, x*50, 1, (x*50) + 49, 29);
	  } else {
	    rect3d_down(screen, x*50, 1,(x*50)+49, 29);
	    button_down = true;
	    pressed_button = x;
	  }
	  
	  temp_button = current_button;
	  show_mouse(screen);
	}
    }
  }
  restore_scr(&temp_scr);
  return 0;
}

// draws all Shapes to screen and checks the mouse
void
redraw (std::vector<Shape*>&a, int last_obj)
{
  BITMAP *temp_scr;
  temp_scr = create_bitmap(SCREEN_W, SCREEN_H);

  if (background)
    stretch_blit(background, temp_scr,
		 0, 0,
		 background->w, background->h,
		 0, 0, SCREEN_W, SCREEN_H);
  else
    clear(temp_scr);

  for (int i = 0; i < last_obj; ++i) {
    //    std::cerr << "Redraw: " << int(i) << "Shape drawn" << std::endl;
    a[i]->draw(temp_scr);
  }

  //vsync();
  show_mouse (NULL);

  blit(temp_scr, screen,
       0,0,
       0,0,
       SCREEN_W, SCREEN_H);
  
  destroy_bitmap(temp_scr);
  
  status_line.refresh ();

  show_mouse (screen);
}


// draws all Shapes to screen

void
_redraw (std::vector<Shape*>&a, int last_obj)
{
  BITMAP *temp_scr;
  temp_scr = create_bitmap(SCREEN_W, SCREEN_H);

  if (background) {
    stretch_blit(background, temp_scr,
		 0, 0, background->w, background->h,
		 0, 0, SCREEN_W, SCREEN_H);
  } else {
    clear(temp_scr);
  }
    
  for (int i = 0; i < last_obj; ++i)
    a[i]->draw (temp_scr);

  blit(temp_scr, screen, 0,0,0,0,SCREEN_W, SCREEN_H);

  destroy_bitmap(temp_scr);
  status_line.refresh ();
}

