// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <iostream>
#include <stdio.h>
#include <allegro.h>
#include "add_aleg.hh"
#include "defs.hh"
#include "menu.hh"

void rect3d(BITMAP *scr, int x1, int y1, int x2, int y2);

// simple function to draw a 3d looking rectangel,
// to build the graphicel base for the menu
void rect3d(BITMAP *scr, int x1, int y1, int x2, int y2)
{
  rectfill(scr, x1, y1, x2, y2, MENU_BOX_COLOR);
  rect(scr, x1-1, y1-1, x2+1, y2+1, MENU_DBOX_COLOR);
  /*
  line(scr, x1, y1, x1, y2, MENU_LBOX_COLOR);
  line(scr, x1, y1, x2, y1, MENU_LBOX_COLOR);

  line(scr, x2, y1, x2, y2, MENU_DBOX_COLOR);
  line(scr, x1, y2, x2, y2, MENU_DBOX_COLOR);
  */
}

// default constructer, only for experimetal purpouse here
// normaly a menu should be initialisesed by an string array
Menu::Menu()
{
  int i;
  int temp_width=0;
  item = new char* [10];

  for(i=0; i<5; ++i) {
    item[i] = new char [255];
    sprintf(item[i], "Item %d", i + 1);

    temp_width = strlen(item[i]) * 8 + 1;

    if (temp_width > width)
      width = temp_width;
  }
  items = i;
}

// copys the menu structure from a string array
Menu::Menu(const char** str)
{

  int i;
  int temp_width=0;
  int number_of_items;

  for(i=0; str[i] != NULL; ++i);
  number_of_items = i;

  item = new char* [number_of_items];

  for(i=0; str[i] != NULL; ++i) {
    item[i] = new char [strlen(str[i])+1];
    strcpy(item[i], str[i]);
    temp_width = strlen(item[i]) * 8 + 1;

    if (temp_width > width)
      width = temp_width;
  }
  items = i;
}

// destroyes all allocated memory (i hope )
Menu::~Menu()
{
  // clog << "Menu: Trying to free memory" << std::endl;
  for(int i=0; i<items; ++i)
    delete[] item[i];
  delete[] item;
  //  clog << "Menu: All memory freed" << std::endl;
}

// checks over which menu item the mouse is and return
// the number as an int
int Menu::show()
{
  BITMAP *temp_scr;
  int value = 1;
  int temp_value = 2;

  // backup screen, draw the menu and
  // set text mode to transparent
  backup_scr(&temp_scr);
  draw();

  while(!mouse_button1_pressed())
    {
      for(value=0; value < items; ++value) {
	if (mouse_x > mx1 && mouse_x < mx1 + width
	    && mouse_y > my1 && mouse_y < my1 + (9 * (value+1))) {
	  break;
	}
      }

      if (value == items && temp_value != value) {
	if (temp_value < items && temp_value >=0) {
	  show_mouse(NULL);
	  textout_ex(screen,
                     font,
                     item[temp_value],
                     mx1, my1 + temp_value * 9, MENU_TEXT_COLOR, -1);
	  show_mouse(screen);
	}
	temp_value = value;
      }

      if (value>=0 && value <items && temp_value != value) {
	show_mouse(NULL);
	if (temp_value >= 0 && temp_value < items)
	  textout_ex(screen,
                     font,
                     item[temp_value],
                     mx1, my1 + temp_value * 9, MENU_TEXT_COLOR, -1);

	textout_ex(screen,
                   font,
                   item[value],
                   mx1, my1 + value * 9, MENU_HTEXT_COLOR, -1);

	show_mouse(screen);
	temp_value = value;
      }
    }

  restore_scr(&temp_scr);

  return value;
}

// draws the graphical menu
void Menu::draw()
{
  mx1 = mouse_x;
  my1 = mouse_y;

  show_mouse(NULL);

  rect3d(screen, mx1-2, my1-2, mx1 + width, my1 + (items * 9));

  for(int i=0; i < items; ++i)
    textout_ex(screen, font, item[i], mx1, my1 + i*9, MENU_TEXT_COLOR, -1);

  show_mouse(screen);
}

/* EOF */
