#ifdef __cplusplus

#include <SoftwareSerial.h>

class Lcd420 {
  public:
  Lcd420(int lcd_rx_pin, int lcd_tx_pin);
  ~Lcd420();
  void clearLine(int line_num);
  void clearScreen();
  void writeChars(char * c, int buff_size);
  void setBlinkingCursor();
  void moveCursorToLine(int line_num);
  void moveCursor(int pos);

  private:
  SoftwareSerial * myLcd;
  int getStartOfLine(int line);
  int remapCursorPosition(int pos);

};

#endif // __cplusplus

typedef void* CLcd420;

extern "C" CLcd420 new_CLcd420(int lcd_tx_pin, int lcd_rx_pin);
extern "C" void del_CLcd420(CLcd420);
extern "C" void clear_line(CLcd420, int line_num);
extern "C" void clear_screen(CLcd420);
extern "C" void write_chars(CLcd420, char * c, int buff_size);
extern "C" void set_blinking_cursor(CLcd420);
extern "C" void move_cursor_to_line(CLcd420,int line_num);
extern "C" void move_cursor(CLcd420, int pos);
