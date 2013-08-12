// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef MENU_HH
#define MENU_HH

class Menu
{
public:
  Menu();
  Menu(const char** str);
  ~Menu();
  
  int show();
private:
  void draw();
  char **item;
  int items;
  int mx1, mx2, my1, my2;
  int width;
};

#endif // #ifndef MENU_HH

/* EOF */
