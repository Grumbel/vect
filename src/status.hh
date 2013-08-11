// Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>, see README.TXT for details

#ifndef STATUS_HH
#define STATUS_HH

class Status_Line
{
public:
  int current_obj;
  int    last_obj;
  
  void print(char *str);
  void refresh();
private:
  char current_str[256];
};

extern Status_Line status_line;

#endif // #ifndef STATUS_HH

/* EOF */
