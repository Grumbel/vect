// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef GROUP_HH
#define GROUP_HH

#include <cstdio>
#include <vector>

class Group : public Shape, public vector<Shape*>
{
public:
  Group();
  Group(const vector<Shape*> &);
  ~Group();

  void draw(BITMAP *);
  void move(int x, int y);
  void save(FILE* out);
  void load(FILE* in);
  void draw_col(BITMAP* scr, int col);
  void scale();
  void rotate();
  void draw_rect(BITMAP* scr);
  Shape* copy();
  bool is_in(int, int, int, int);
  unsigned int size();
  Shape* at(unsigned int);
  Shape* operator[](unsigned int);
  void get_border();
private:
  int x1_pos, x2_pos, y1_pos, y2_pos;
  vector<Shape*> a;
};

#endif // #ifndef GROUP_HH

/* EOF */
