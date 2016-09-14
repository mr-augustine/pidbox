/*
 * file: pidbox.ino
 * created: 20160913
 * author(s): mr-augustine, khaosduke
 *
 */
#include "Lcd420.h"
#include "buttons.h"

#define LCD_RX_PIN 2
#define LCD_TX_PIN 3
// #define INCREMENT_BUTTON_PIN 4
#define LED_PIN 13

volatile byte led_state = HIGH;
// volatile byte prev_button_state;
// volatile byte curr_button_state;

CLcd420 lcd = new_CLcd420(LCD_TX_PIN, LCD_RX_PIN);

char line_0[NUM_COLUMNS];
char line_1[NUM_COLUMNS];
char line_2[NUM_COLUMNS];
char line_3[NUM_COLUMNS];

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(INCREMENT_BUTTON_PIN, INPUT);
  // attachInterrupt((INCREMENT_BUTTON_PIN), increment, FALLING);
  // prev_button_state = digitalRead(INCREMENT_BUTTON_PIN);

  delay(500);
  clear_screen(lcd);

  // write_chars(lcd, "hey!", 4);
  // delay(3000);
  // clear_screen(lcd);
  //
  // move_cursor_to_line(lcd, 2);
  // set_blinking_cursor(lcd);
  //
  // delay(3000);
  // move_cursor(lcd, 17);
  // write_chars(lcd, "@", 1);

  float k_prop = 3.14567890;
  float k_integ = 6.56789321;
  float k_deriv = 9.867530909;

  delay(3000);
  clear_screen(lcd);
  init_line_buffers();

  strcpy(line_0, " P: ");
  dtostrf(k_prop, 2, 4, &line_0[strlen(line_0)]);

  strcpy(line_1, " I: ");
  dtostrf(k_integ, 2, 4, &line_1[strlen(line_1)]);

  strcpy(line_2, "-D: ");
  dtostrf(k_deriv, 2, 4, &line_2[strlen(line_2)]);

  strcpy(line_3, "+/-[0.00001]  Ready!");

  delay(3000);
  move_cursor_to_line(lcd, 0);
  write_chars(lcd, line_0, 20);
  move_cursor_to_line(lcd, 1);
  write_chars(lcd, line_1, 20);
  move_cursor_to_line(lcd, 2);
  write_chars(lcd, line_2, 20);
  move_cursor_to_line(lcd, 3);
  write_chars(lcd, line_3, 20);

  move_cursor_to_line(lcd, 2);

  Serial.println(line_0[0], HEX);
  Serial.println(line_0[2], HEX);
  Serial.println(line_0[19], HEX);
  Serial.println("----");

  init_buttons();
}

void loop() {
  update_all_buttons();

  if (button_pressed(&increment)) {
    do_increment();
  }

  digitalWrite(LED_PIN, led_state);
}

void init_line_buffers(void) {
  memset(line_0, 0x20, NUM_COLUMNS);
  memset(line_1, 0x20, NUM_COLUMNS);
  memset(line_2, 0x20, NUM_COLUMNS);
  memset(line_3, 0x20, NUM_COLUMNS);
}

// void update_button(void) {
//   curr_button_state = digitalRead(INCREMENT_BUTTON_PIN);
//
//   if (prev_button_state == HIGH && curr_button_state == LOW) {
//     increment();
//   }
//
//   prev_button_state = curr_button_state;
//   // if (digitalRead(INCREMENT_BUTTON_PIN) == LOW) {
//   //   led_state = LOW;
//   // } else {
//   //   led_state = HIGH;
//   // }
// }

void do_increment(void) {
  led_state = !led_state;
}
