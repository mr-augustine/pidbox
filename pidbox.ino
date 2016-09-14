/*
 * file: pidbox.ino
 * created: 20160913
 * author(s): mr-augustine, khaosduke
 *
 */
#include "Lcd420.h"

#define LCD_RX_PIN 2
#define LCD_TX_PIN 3

CLcd420 lcd = new_CLcd420(LCD_TX_PIN, LCD_RX_PIN);

void setup() {
  delay(500);
  clear_screen(lcd);

  write_chars(lcd, "hey!", 4);
  delay(3000);
  clear_screen(lcd);

  move_cursor_to_line(lcd,2);
  set_blinking_cursor(lcd);

  delay(3000);
  move_cursor(lcd,17);
  write_chars(lcd,"@",1);

  float k_prop = 3.14567890;
  char line_0[20];
  memset(line_0, ' ', 20);

  strcpy(line_0, "P: ");
  dtostrf(k_prop, 2, 4, &line_0[strlen(line_0)]);

  delay(3000);
  move_cursor_to_line(lcd, 3);
  write_chars(lcd, line_0, 20);
}

void loop() {

}
