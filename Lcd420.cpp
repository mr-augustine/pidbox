#include "Lcd420.h"

Lcd420::Lcd420(int lcd_tx_pin, int lcd_rx_pin) : currentField {0} {
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
  replaceNullWithSpace(c, buff_size);
  myLcd->write(c, buff_size);
}

void Lcd420::replaceNullWithSpace(char * c, int buff_size) {
  for (int i = 0; i < buff_size; i++) {
    if (c[i] == '\0') {
      c[i] = ' ';
    }
  }
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
  //int real_pos = pos;
  myLcd->write(SPECIAL_CMD);
  myLcd->write(CURSOR_PREFIX+real_pos);
}

void Lcd420::moveCursorToField(int field_num) {
  currentField = field_num;

  switch (field_num) {
    case 0:
      moveCursor(FIELD_0_POS);
      break;
    case 1:
      moveCursor(FIELD_1_POS);
      break;
    case 2:
      moveCursor(FIELD_2_POS);
      break;
    case 3:
      moveCursor(FIELD_3_POS);
      break;
    case 4:
      moveCursor(FIELD_4_POS);
      break;
    default:
      moveCursor(FIELD_0_POS);
      currentField = 0;
  }
}

void Lcd420::moveCursorToNextField() {
  int nextField = (currentField + NUM_FIELDS + 1) % NUM_FIELDS;

  moveCursorToField(nextField);
}

void Lcd420::moveCursorToPrevField() {
  int prevField = (currentField + NUM_FIELDS - 1) % NUM_FIELDS;

  moveCursorToField(prevField);
}

int Lcd420::getCurrentField() {
  return currentField;
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

void move_cursor_to_field(CLcd420 lcd420, int field_num) {
  reinterpret_cast<Lcd420*>(lcd420)->moveCursorToField(field_num);
}

void move_cursor_to_next_field(CLcd420 lcd420) {
  reinterpret_cast<Lcd420*>(lcd420)->moveCursorToNextField();
}

void move_cursor_to_prev_field(CLcd420 lcd420) {
  reinterpret_cast<Lcd420*>(lcd420)->moveCursorToPrevField();
}

void move_cursor_to_line(CLcd420 lcd420, int line_num) {
  reinterpret_cast<Lcd420*>(lcd420)->moveCursorToLine(line_num);
}

void move_cursor(CLcd420 lcd420, int pos) {
  reinterpret_cast<Lcd420*>(lcd420)->moveCursor(pos);
}

int get_current_field(CLcd420 lcd420) {
   return reinterpret_cast<Lcd420*>(lcd420)->getCurrentField();
}
