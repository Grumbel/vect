/******************/
/*     __     _	  */
/* |  /  | __| |  */
/* | /  /_| _   > */
/* |/  / _|<_| |  */
/* |__/___|__|_|  */
/*                */
/**************[G]*/
/*  Ingo Ruhnke   */
/* grumbel@gmx.de */
/******************/

#include <iostream.h>
#include <stdio.h>
#include <unistd.h>
#include <allegro.h>

//#include "vector.hh"
#include "objects.hh"
#include "status.hh"
#include "color.hh"

Status_Line status_line;

int redraw(Object **a, int last_obj);
RGB get_color();
int update_color(void *dp3, int val);
int _redraw(Object **a, int last_obj);

int update_color(void *dp3, int val){}

int _redraw(Object **a, int last_obj)
{
  for(int i=0; i<last_obj; ++i) {
    a[i]->draw(screen);
  }

  status_line.refresh();
}

int redraw(Object **a, int last_obj)
{
  vsync();

  show_mouse(NULL);

  for(int i=0; i<last_obj; ++i) {
    a[i]->draw(screen);
  }
  status_line.refresh();
  
  show_mouse(screen);
}

RGB get_color()
{
  BITMAP *temp_scr;
  RGB color;
  PALETTE pal;
  
  DIALOG select_color[] = {
    /*(dialogproc)(x)(y)(w)(h)(fg)(bg)(key)(flags)(d1)(d2) (dp)(dp2)(dp3) */
    { d_slider_proc,  32, 32,256,16,  1,0,0,0,63,0,NULL, update_color, 0 },
      { d_slider_proc,  32, 64,256,16,  2,0,0,0,63,0,NULL, update_color, 0 },
      { d_slider_proc,  32, 96,256,16,  4,0,0,0,63,0,NULL, update_color, 0 },
      { d_text_proc,   300, 38,  0, 0,255,0,0,0, 0,0,"R"  },
      { d_text_proc,   300, 70,  0, 0,255,0,0,0, 0,0,"G"  },
      { d_text_proc,   300,102,  0, 0,255,0,0,0, 0,0,"B"  },
      { NULL,            0,  0,  0, 0,  0,0,0,0,0,0,NULL }
  };
  
  backup_scr(&temp_scr);
  get_palette(pal);
  
  set_palette(desktop_palette);
  do_dialog(select_color, -1);  

  color.r = select_color[0].d2;
  color.g = select_color[1].d2;
  color.b = select_color[2].d2;

  set_palette(pal);
  restore_scr(&temp_scr);
  set_color(15, &color);
  return color;
}

int main(int argc, char *argv[])
{
  int c;
  int scr_w=640, scr_h=480;
  PALETTE pal;
  
  opterr = 0;

  while ((c=getopt(argc, argv, "w:h:")) != -1) {
    switch (c)
      {
	case 'w':
	  scr_w = atoi(optarg);
	  break;
	case 'h':
	  scr_h = atoi(optarg);
	  break;
	case '?':
	  printf("Unknown option %c\n\n"
		 "Usage: VECTOR.EXE -w XXX -h XXX\n"
		 , c);
	  exit(0);
      }
  }

  Object *a[50];

  int current_obj=0;
  int last_obj=0;

  allegro_init();
  install_keyboard();

  install_timer();
  install_mouse();

  set_gfx_mode(GFX_AUTODETECT, scr_w, scr_h,0,0);

  for(int i=0; i<256; ++i) {
    pal[i].r = pal[i].b = pal[i].g = i/4;
  }

  set_palette(pal);
  show_mouse(screen);

  while((c >> 8) != KEY_Q) {

    while(!keypressed());

    c = readkey();

    switch (c >> 8)
      {
	case KEY_Q:
	  break;

	case KEY_D:
	  redraw(a, last_obj);
	  break;

	case KEY_K:
	  status_line.print("Create new Circle");
	  
	  a[last_obj]  = new Circle;
	  a[last_obj++]->create_new();
	  break;

	case KEY_L:
	  status_line.print("Create new Line");
	  
	  a[last_obj] = new Line;
	  a[last_obj++]->create_new();
	  break;

	case KEY_SPACE:
	  show_mouse(NULL);
	  clear(screen);
	  show_mouse(screen);
	  break;
	  
	case KEY_F5: {
	  FILE *in;
	  char filename[256]={""};
	  char type[256];
	  char buf[256];
	  int i=0;
	  BITMAP *temp_scr;

	  backup_scr(&temp_scr);
	  
	  if (file_select("Load", filename, NULL))
	    {
	      if (!(in = fopen(filename, "rt"))) {
		perror(filename);
		exit(1);
	      }
	      last_obj = 0;
	      while(fscanf(in, "[%[^]]]\n", type) == 1)
		{
		  if (!strcmp(type, "Circle"))
		    {
		      a[last_obj] = new Circle;
		      a[last_obj++]->load(in);
		    }
		  else if (!strcmp(type, "Polygon"))
		    {
		      a[last_obj] = new Polygon;
		      a[last_obj++]->load(in);
		    }
		  else if (!strcmp(type, "Line"))
		    {
		      a[last_obj] = new Line;
		      a[last_obj++]->load(in);
		    }
		  else
		    {
		      fprintf(stderr, "Typ [%s] unknown\n", type);
		      exit(1);
		    }
		}
	      fclose(in);
	    }
	  restore_scr(&temp_scr);
	}
	  break;

	case KEY_F6: {
	  FILE *out;
	  char filename[256]={""};
	  BITMAP *temp_scr;

	  backup_scr(&temp_scr);
	  
	  if (file_select("Save", filename, NULL))
	    {
	      if ((exists(filename)
		  && _alert("File exist", get_filename(filename),
			     "overwrite",
			     "yes", "no", 'y', 'n') == 1)
		  || !exists(filename))
		{
		  out = fopen(filename, "wt");
		  
		  for(int i=0; i<last_obj; ++i) 
		    a[i]->save(out);
		  
		  fclose(out);
		}
	    }
	  restore_scr(&temp_scr);
	}
	  break;
	  
	case KEY_TAB:
	  if (++current_obj >= last_obj)
	    current_obj = 0;
          clear(screen);
	  redraw(a, last_obj);
	  
	  show_mouse(NULL);
	  a[current_obj]->draw_rect(screen);
	  show_mouse(screen);

	  while(key[KEY_TAB]);
	
	  break;

	case KEY_C:
	  get_color();
	  a[current_obj]->color = rand() % 255;
	  redraw(a, last_obj);
	  break;

	case KEY_U:
	  get_palette(pal);
	  edit_palette(pal);
	  break;
	  
	case KEY_P:
	  status_line.print("Create new Polygon");
	  
	  a[last_obj] = new Polygon;
	  current_obj = last_obj;
	  a[last_obj++]->create_new();
  	  break;
	  
	case KEY_M:
	  {
	    int x;
	    int y;

	    status_line.print("Verschiebe Objekt");
	    
	    show_mouse(NULL);
	    get_mouse_mickeys(&x,&y);

	    while(!mouse_button1_pressed())
	      {
		do {
		  get_mouse_mickeys(&x,&y);
		} while(!(mouse_b & 1) && x == 0 && y == 0);
		
		a[current_obj]->move(x, y);
		clear(screen);
		_redraw(a, last_obj);
	      }

	    while(key[KEY_M]){}
	    show_mouse(screen);
	    clear_keybuf();
	  }
	break;
	default:
	  cout << "Unkown key pressed" << endl;
      }
  }

  allegro_exit();
  return 0;
}

