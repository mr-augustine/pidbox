#ifdef __cplusplus

#include <SoftwareSerial.h>


#define LINE_0_START_ADDR 0
#define LINE_1_START_ADDR 64
#define LINE_2_START_ADDR 20
#define LINE_3_START_ADDR 84

#define NUM_FIELDS  5
#define FIELD_0_POS 0
#define FIELD_1_POS 20
#define FIELD_2_POS 40
#define FIELD_3_POS 60
#define FIELD_4_POS 73

#define CMD_RETURN_HOME         0x02
#define CMD_CLEAR_DISPLAY       0x01
#define SPECIAL_CMD             0xFE
#define CMD_BLINKING_CURSOR_ON  0x0D
#define CMD_UNDERSCORE_CURSOR_ON 0x0E
#define CURSOR_PREFIX           0x80

#define BACKLIGHT_CMD 0x7C
#define CMD_BACKLIGHT_ON 157
#define CMD_BACKLIGHT_OFF 128

#define CMD_DISPLAY_ON 0x0C
#define CMD_DISPLAY_OFF 0x08

#define NUM_COLUMNS 20
#define NUM_ROWS    4

#define DELAY_LENGTH 10

class Lcd420 {
  public:
  Lcd420(int lcd_rx_pin, int lcd_tx_pin);
  ~Lcd420();
  void clearLine(int line_num);
  void clearScreen();
  void writeChars(char * c, int buff_size);
  void setBlinkingCursor();
  void setUnderscoreCursor();
  void turnBacklightOn();
  void turnBacklightOff();
  void turnDisplayOn();
  void turnDisplayOff();
  void moveCursorToLine(int line_num);
  void moveCursorToField(int field_num);
  void moveCursorToNextField();
  void moveCursorToPrevField();
  void moveCursor(int pos);
  int getCurrentField();

  private:
  int currentField;

  SoftwareSerial * myLcd;
  int getStartOfLine(int line);
  int remapCursorPosition(int pos);
  void replaceNullWithSpace(char * c, int buff_size);
  void delay(uint16_t millisec);

};

#endif // __cplusplus

typedef void* CLcd420;

extern "C" CLcd420 new_CLcd420(int lcd_tx_pin, int lcd_rx_pin);
extern "C" void del_CLcd420(CLcd420);
extern "C" void clear_line(CLcd420, int line_num);
extern "C" void clear_screen(CLcd420);
extern "C" void write_chars(CLcd420, char * c, int buff_size);
extern "C" void set_blinking_cursor(CLcd420);
extern "C" void set_underscore_cursor(CLcd420);
extern "C" void turn_backlight_on(CLcd420);
extern "C" void turn_backlight_off(CLcd420);
extern "C" void turn_display_on(CLcd420);
extern "C" void turn_display_off(CLcd420);
extern "C" void move_cursor_to_field(CLcd420, int field_num);
extern "C" void move_cursor_to_line(CLcd420, int line_num);
extern "C" void move_cursor_to_next_field(CLcd420);
extern "C" void move_cursor_to_prev_field(CLcd420);
extern "C" void move_cursor(CLcd420, int pos);
extern "C" int get_current_field(CLcd420);
