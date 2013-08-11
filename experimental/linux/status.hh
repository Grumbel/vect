
class Status_Line
{
public:
  void print(char *str);
  void refresh();
private:
  char current_str[256];
};
