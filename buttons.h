#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include <stdint.h>

#define INCREMENT_BUTTON_PIN  7
#define DECREMENT_BUTTON_PIN  6
#define SELECT_BUTTON_PIN     5
#define KILL_BUTTON_PIN       4

typedef struct {
  uint8_t pin;
  uint8_t prev_state;
  uint8_t curr_state;
} button_t;

volatile button_t increment;
volatile button_t decrement;
volatile button_t select;
volatile button_t killswitch;

uint8_t button_pressed(button_t * button);
void init_buttons(void);
void update_all_buttons(void);
void update_button_state(button_t * button);

void init_buttons(void) {
  increment.pin = INCREMENT_BUTTON_PIN;
  decrement.pin = DECREMENT_BUTTON_PIN;
  select.pin = SELECT_BUTTON_PIN;
  killswitch.pin = KILL_BUTTON_PIN;

  pinMode(increment.pin, INPUT);
  pinMode(decrement.pin, INPUT);
  pinMode(select.pin, INPUT);
  pinMode(killswitch.pin, INPUT);

  update_all_buttons();

  increment.prev_state = increment.curr_state;
  decrement.prev_state = decrement.curr_state;
  select.prev_state = select.curr_state;
  killswitch.prev_state = killswitch.curr_state;
}

// Here we are using a pointer to a volatile button_t, whereas the text:
// "button_t * volatile button" means we are using a volatile pointer to
// a non-volatile button_t
void update_button_state(volatile button_t * button) {
  uint8_t state = digitalRead(button->pin);

  button->prev_state = button->curr_state;
  button->curr_state = state;
}

void update_all_buttons(void) {
  update_button_state(&increment);
  update_button_state(&decrement);
  update_button_state(&select);
  update_button_state(&killswitch);
}

uint8_t button_pressed(volatile button_t * button) {
  // Serial.println("In button_pressed()");
  if (button->prev_state == HIGH &&
      button->curr_state == LOW) {
    // Serial.println("button changed");
    return 1;
  }
  // Serial.println("no change");
  return 0;
}

#endif // _BUTTONS_H_
