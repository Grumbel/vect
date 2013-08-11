// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef ADD_ALEG_HH
#define ADD_ALEG_HH

bool mouse_button1_pressed();
bool mouse_button2_pressed();
bool mouse_button3_pressed();
void input_line(char *buf, int x, int y, int color);
bool rect_col(int ax1, int ax2, int ay1, int ay2,
              int bx1, int bx2, int by1, int by2);
void make_gamma_pal(PALETTE srcpal, PALETTE destpal, float gamma);
void scroll(char **str, int lines, int speed);

void backup_screen();
void backup_scr(BITMAP **tmp);
void restore_scr(BITMAP **tmp);
void _backup_scr(BITMAP **tmp);
void _restore_scr(BITMAP **tmp);

int  _alert(char *s1, char *s2, char *s3, char *b1, char *b2, int c1, int c2);
RGB  rgb_color(unsigned char r, unsigned char g, unsigned char b);
void draw_pcx(BITMAP *scr, char *filename, int x, int y);
void draw_pcx_sprite(BITMAP *scr, char *filename, int x, int y);
bool mouse_between(int x1, int y1, int x2, int y2);
void show_text(char **str, const int max_lines);
void edit_text_screen(BITMAP *screen);
void vertex(BITMAP *scr, int x, int y, int size, int color);

#endif // #ifndef ADD_ALEG_HH

/* EOF */
