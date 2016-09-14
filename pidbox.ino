/*
 * file: pidbox.ino
 * created: 20160913
 * author(s): mr-augustine, khaosduke
 *
 */
// #include <SoftwareSerial.h>
// #include "lcd420.h"
#include "Lcd420.h"

#define LCD_RX_PIN 2
#define LCD_TX_PIN 3

CLcd420 lcd = new_CLcd420(LCD_TX_PIN, LCD_RX_PIN);

void setup() {
  delay(500);
  clear_screen(lcd);
}

void loop() {

}
