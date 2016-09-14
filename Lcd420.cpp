#include "Lcd420.h"

#define LINE_0_START_ADDR 0
#define LINE_1_START_ADDR 64
#define LINE_2_START_ADDR 20
#define LINE_3_START_ADDR 84


#define CMD_RETURN_HOME         0x02
#define CMD_CLEAR_DISPLAY       0x01
#define SPECIAL_CMD             0xFE
#define CMD_BLINKING_CURSOR_ON  0x0F
#define CURSOR_PREFIX           0x80


Lcd420::Lcd420(int lcd_tx_pin, int lcd_rx_pin) {
  myLcd = new SoftwareSerial(lcd_tx_pin, lcd_rx_pin);
  myLcd->begin(9600);
}

Lcd420::~Lcd420() {
  delete myLcd;
}

int Lcd420::getStartOfLine(int line) {
  int cursorPos = 0;

  switch (line) {
    case 0:
      cursorPos = 0;
      break;
    case 1:
      cursorPos = 64;
      break;
    case 2:
      cursorPos = 20;
      break;
    case 3:
      cursorPos = 84;
      break;
    default:
      cursorPos = 0;
  }

  return cursorPos;
}

void Lcd420::clearLine(int line_num) {
  int cursorPos = getStartOfLine(line_num);

  myLcd->write(CURSOR_PREFIX + cursorPos);
  myLcd->write("                    ");
}

void Lcd420::clearScreen() {
  myLcd->write(SPECIAL_CMD);
  myLcd->write(CMD_CLEAR_DISPLAY);

  myLcd->write(SPECIAL_CMD);
  myLcd->write(CMD_RETURN_HOME);
}

void Lcd420::writeChars(char * c, int buff_size) {
  myLcd->write(c, buff_size);
}

void Lcd420::setBlinkingCursor() {
  myLcd->write(SPECIAL_CMD);
  myLcd->write(CMD_BLINKING_CURSOR_ON);
}

void Lcd420::moveCursorToLine(int line_num) {
  myLcd->write(SPECIAL_CMD);

  int cursorPos = getStartOfLine(line_num);

  myLcd->write(CURSOR_PREFIX + cursorPos);
}

//Maps our intended position to controller's addressing
int Lcd420::remapCursorPosition(int pos) {
  if( pos >= 0 &&  pos <= 19) {
    return pos;
  } else if( pos >= 20 && pos <= 39) {
    return (LINE_1_START_ADDR - 20) + pos;
  } else if( pos >= 40 && pos <= 59) {
    return (LINE_2_START_ADDR - 40) + pos;
  } else if( pos >=60 && pos <=79) {
    return (LINE_3_START_ADDR - 60) + pos;
  } else {
    return 0;
  }
}

void Lcd420::moveCursor(int pos) {
  int real_pos = remapCursorPosition(pos);

  myLcd->write(SPECIAL_CMD);
  myLcd->write(CURSOR_PREFIX+real_pos);
}
/// C Below ///

CLcd420 new_CLcd420(int lcd_tx_pin, int lcd_rx_pin) {
  return reinterpret_cast<void*>(new Lcd420(lcd_tx_pin, lcd_rx_pin));
}

void del_CLcd420(CLcd420 lcd420) {
  delete reinterpret_cast<Lcd420*>(lcd420);
}

void clear_line(CLcd420 lcd420, int line_num) {
  reinterpret_cast<Lcd420*>(lcd420)->clearLine(line_num);
}

void clear_screen(CLcd420 lcd420) {
  reinterpret_cast<Lcd420*>(lcd420)->clearScreen();
}

void write_chars(CLcd420 lcd420, char * c, int buff_size) {
  reinterpret_cast<Lcd420*>(lcd420)->writeChars(c, buff_size);
}

void set_blinking_cursor(CLcd420 lcd420) {
  reinterpret_cast<Lcd420*>(lcd420)->setBlinkingCursor();
}

void move_cursor_to_line(CLcd420 lcd420, int line_num) {
  reinterpret_cast<Lcd420*>(lcd420)->moveCursorToLine(line_num);
}

void move_cursor(CLcd420 lcd420, int pos) {
  reinterpret_cast<Lcd420*>(lcd420)->moveCursor(pos);
}
