// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

class Circle : public Shape
{
public:
  bool filled;
  Circle(int col = 255);
  ~Circle();

  void rotate();
  void move(int xm, int ym);
  void create_new();
  void draw_rect(BITMAP *scr);
  void draw(BITMAP *scr);
  void save(FILE *out);
  void show_vertex();
  void move_vertex();
  void load(FILE *in);
  void draw_vertex();
  void draw_col(BITMAP *scr, int col);
  void copy(Shape*);
  void scale();
  void set_color(int);
  void get_border();
private:
  int   x, y;
  int radius;
};

/* EOF */
