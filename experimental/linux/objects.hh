#include <stdio.h>
#include <allegro.h>
#include "add_aleg.hh"

struct Point {
  int x;
  int y;
};

class Object {
public:
  virtual ~Object(){}
  
  int color;
  virtual void draw(BITMAP *scr){}
  virtual void move(int x, int y){}
  virtual void create_new(){}
  virtual void draw_rect(BITMAP *scr){}
  virtual void save(FILE *out){}
  virtual void load(FILE *in){}
  
private:
};

class Circle : public Object {
public:
  Circle();
  ~Circle();

  void move(int xm, int ym);
  void create_new();
  void draw_rect(BITMAP *scr);
  void draw(BITMAP *scr);
  void save(FILE *out);
  void load(FILE *in);
  
private:
  int   x, y;
  int radius;
};

class Line : public Object { 
public:
  Line();
  ~Line();

  void move(int x, int y);
  void create_new();
  void draw_rect(BITMAP *scr);
  void draw(BITMAP *scr);
  void save(FILE *out);
  void load(FILE *in);

  Point *p;
  int last_point;
  
private:
  void create_new_dot(int x, int y);
};


class Polygon : public Object {
public:
  Polygon();
  
  void create_new();
  void move(int x, int y);
  void draw_rect(BITMAP *scr);
  void draw(BITMAP *scr);
  void save(FILE *out);
  void load(FILE *in);
  Point *p;
  int last_point;
    
private:
  void create_new_dot(int x, int y);
};
