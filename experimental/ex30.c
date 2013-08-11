/* 
 *    Example program for the Allegro library, by Shawn Hargreaves.
 *
 *    This program shows how to convert colors between the RGB and HSV
 *    representations.
 */


#include <stdlib.h>
#include <stdio.h>

#include "allegro.h"



/* slider types (R, G, B, and H, S, V) */
#define S_R    0
#define S_G    1
#define S_B    2
#define S_H    3
#define S_S    4
#define S_V    5



/* the current color values */
static int colors[6] =
{
   255,     /* red */
   255,     /* green */
   255,     /* blue */
   0,       /* hue */
   0,       /* saturation */
   255      /* value */
};



/* helper for changing one of the color values */
int update_color(void *dp3, int val)
{
   int type = ((unsigned long)dp3 - (unsigned long)colors) / sizeof(colors[0]);
   int r, g, b;
   float h, s, v;
   RGB rgb;

   if (colors[type] != val) {
      colors[type] = val;

      if ((type == S_R) || (type == S_G) || (type == S_B)) {
	 /* convert RGB color to HSV */
	 r = colors[S_R];
	 g = colors[S_G];
	 b = colors[S_B];

	 rgb_to_hsv(r, g, b, &h, &s, &v);

	 colors[S_H] = h * 255.0 / 360.0;
	 colors[S_S] = s * 255.0;
	 colors[S_V] = v * 255.0;
      }
      else {
	 /* convert HSV color to RGB */
	 h = colors[S_H] * 360.0 / 255.0;
	 s = colors[S_S] / 255.0;
	 v = colors[S_V] / 255.0;

	 hsv_to_rgb(h, s, v, &r, &g, &b);

	 colors[S_R] = r;
	 colors[S_G] = g;
	 colors[S_B] = b;
      }

      /* set the screen background to the new color */
      rgb.r = colors[S_R]/4;
      rgb.g = colors[S_G]/4;
      rgb.b = colors[S_B]/4;

      vsync();
      set_color(0, &rgb);
   } 

   return D_O_K;
}



/* gui object procedure for the color selection sliders */
int my_slider_proc(int msg, DIALOG *d, int c)
{
   int *color = (int *)d->dp3;

   switch (msg) {

      case MSG_START:
	 /* initialise the slider position */
	 d->d2 = *color;
	 break;

      case MSG_IDLE:
	 /* has the slider position changed? */
	 if (d->d2 != *color) {
	    d->d2 = *color;
	    show_mouse(NULL);
	    SEND_MESSAGE(d, MSG_DRAW, 0);
	    show_mouse(screen);
	 }
	 break;
   }

   return d_slider_proc(msg, d, c);
}




int main()
{
   allegro_init();
   install_keyboard();
   install_mouse();
   install_timer();

   if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) != 0) {
      allegro_exit();
      printf("Error setting graphics mode\n%s\n\n", allegro_error);
      return 1;
   }

   set_pallete(desktop_pallete);
   clear(screen);

   do_dialog(the_dlg, -1);

   return 0;
}

