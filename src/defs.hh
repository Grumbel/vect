// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details
//
//   This file contains the defines, mostly the
//   color defines, which were used to define
//   the first 16/32 colors of the palette.

#ifndef DEFS_HH
#define DEFS_HH

#define VECT_VERSION  "0.4Beta"

#define TEXT_COLOR           1
#define TEXT_BG_COLOR        2
#define MENU_DBOX_COLOR      3
#define MENU_LBOX_COLOR      4
#define MENU_BOX_COLOR       5
#define MENU_TEXT_COLOR      6
#define MENU_HTEXT_COLOR     7
#define VERTEX_COLOR         8
#define VERTEX_HCOLOR       10
#define LINE_COLOR           9
#define FIXED_COLORS        32

struct Point
{
  int x;
  int y;
};

enum Shape_Type {NONE, POLYGON, LINE, CIRCLE, SHAPE, TEXT, GROUP};

#endif

/* EOF */
