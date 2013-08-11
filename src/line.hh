// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef LINE_HH
#define LINE_HH

class Line : public Shape 
{ 
public:
  Line(int col);
  Line();
  ~Line();
  void rotate();
  void move(int x, int y);
  void create_new();
  void draw_rect(BITMAP *scr);
  void scale();
  void draw(BITMAP *scr);
  void save(FILE *out);
  void load(FILE *in);
  void draw_col(BITMAP *scr, int col);
  void show_vertex();
  void draw_vertex();
  void move_vertex();
  void copy(Shape*);
  void set_color(int);
  void get_border();
  
  Point *p;
  int last_point;
  
private:
  void create_new_dot(int x, int y);
};

#endif // #ifndef LINE_HH

/* EOF */
