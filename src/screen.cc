// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#include <allegro.h>
#include "screen.hh"

Screen::Screen()
{
  scr = 0;
}

Screen::~Screen()
{
  if (scr)
    destroy_bitmap(scr);
}

void
Screen::put()
{
  if (scr) {
    show_mouse(NULL);
    blit(scr, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    show_mouse(screen);
  }
}

void
Screen::get()
{
  if (scr)
    destroy_bitmap(scr);

  scr = create_bitmap(SCREEN_W, SCREEN_H);
  show_mouse(NULL);
  blit(screen, scr, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
  show_mouse(screen);
}

BITMAP*
Screen::bmp()
{
  return scr;
}

/* EOF */
