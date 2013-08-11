// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <stdio.h>
#include <allegro.h>
#include "defs.hh"
#include "status.hh"

void Status_Line::print(const char *str)
{
  rectfill(screen, 0, SCREEN_H - 10, SCREEN_W - 1, SCREEN_H, TEXT_BG_COLOR);
  textout(screen, font, str, 2, SCREEN_H - 10, TEXT_COLOR);
  
  sprintf(current_str,
	  "%-50s - Current_Obj: %3d - Last_Obj: %3d",
	  str, current_obj, last_obj);
}

void Status_Line::refresh()
{
  rectfill(screen, 0, SCREEN_H - 10, SCREEN_W - 1, SCREEN_H, TEXT_BG_COLOR);
  textout(screen, font, current_str, 2, SCREEN_H - 10, TEXT_COLOR);  
}

Status_Line status_line;

/* EOF */
