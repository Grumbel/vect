#include <allegro.h>

#include "status.hh"

void Status_Line::print(char *str)
{
  rectfill(screen, 0, SCREEN_H - 10, SCREEN_W - 1, SCREEN_H, 0);
  textout(screen, font, str, 2, SCREEN_H - 10, 255);
  strcpy(current_str, str);
}

void Status_Line::refresh()
{
  rectfill(screen, 0, SCREEN_H - 10, SCREEN_W - 1, SCREEN_H, 0);
  textout(screen, font, current_str, 2, SCREEN_H - 10, 255);  
}
