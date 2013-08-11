// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef SCREEN_HH
#define SCREEN_HH

#include <allegro.h>

class Screen
{
public:
  Screen();
  ~Screen();
  void get();
  void put();
  BITMAP* bmp();
private:
  BITMAP* scr;
};

#endif // SCREEN_HH

/* EOF */
