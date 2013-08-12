// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef UNSORTED_HH
#define UNSORTED_HH

#include "shape.hh"
#include "status.hh"
#include "menu.hh"

void check_obj(std::vector<Shape*>a, int last_obj);
void redraw(std::vector<Shape*>&a, int last_obj);
void _redraw(std::vector<Shape*>&a, int last_obj);
int  load_file(std::vector<Shape*>&a, int last_obj);
int  load_shape(std::vector<Shape*>&a, int last_obj, char *filename);
void save_file(std::vector<Shape*>&a, int last_obj);
int  get_txt_file(char *filename, char **str);
int  raise_obj(std::vector<Shape*>&a, int last_obj, int current_obj, int pos);
void rect3d_up(BITMAP *scr, int x1, int y1, int x2, int y2);
void rect3d_down(BITMAP *scr, int x1, int y1, int x2, int y2);
void rect3d_n(BITMAP *scr, int x1, int y1, int x2, int y2);
void draw_textbox(BITMAP *scr, int x_pos, int y_pos, const char* str);
void display_text (const char* filename);
void _redraw_col (BITMAP * scr, std::vector<Shape*>&a, int last_obj);
int  display_buttons();

void load_Shape(std::vector<Shape*> &shapes, char* filename);

#endif

/* EOF */
