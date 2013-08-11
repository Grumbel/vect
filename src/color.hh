// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef COLOR_HH
#define COLOR_HH

int  edit_palette(PALETTE pal);
RGB  change_color(void);
int  update_color(void *dp3, int val);
void init_palette(void);
void load_palette(FILE *in);
void save_palette(FILE *out);
void fade_colors(PALETTE pal, int c1, int c2);

#endif // #ifndef COLOR_HH


/* EOF */
