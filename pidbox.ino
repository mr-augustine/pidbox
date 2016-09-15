/*
 * file: pidbox.ino
 * created: 20160913
 * author(s): mr-augustine, khaosduke
 *
 */
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "Lcd420.h"
#include "buttons.h"
#include "fields.h"

#define LCD_RX_PIN 2
#define LCD_TX_PIN 3

#define K_PROP_ADDR 37
#define K_INTEG_ADDR 81
#define K_DERIV_ADDR 127

#define K_PROP_DEFAULT 1
#define K_INTEG_DEFAULT 0
#define K_DERIV_DEFAULT 0

#define MAX_TRIES 5

// If the sentinel value is written to the EEPROM at the specified address, then
// we will pull the pid values from the EEPROM.
#define PIDBOX_INDICATOR_ADDR 27
#define PIDBOX_INDICATOR_SENTINEL 213

#define XBEE_TX_PIN 8
#define XBEE_RX_PIN 9

CLcd420 lcd = new_CLcd420(LCD_TX_PIN, LCD_RX_PIN);

char line_0[NUM_COLUMNS];
char line_1[NUM_COLUMNS];
char line_2[NUM_COLUMNS];
char line_3[NUM_COLUMNS];

field_t k_proportional = {0};
field_t k_integral = {0};
field_t k_derivative = {0};
field_t increment_by = {0};
field_t status = {0};

field_t * all_fields[NUM_FIELDS];

uint8_t pid_buff[3 * sizeof(float)] = {0};
uint8_t status_is_dirty = 0;

SoftwareSerial * myXbee;

void setup() {
  Serial.begin(9600);

  myXbee = new SoftwareSerial(XBEE_TX_PIN, XBEE_RX_PIN);
  myXbee->begin(9600);

  delay(500);
  clear_screen(lcd);

  init_buttons();
  set_all_fields();
  // set_blinking_cursor(lcd);

  turn_backlight_on(lcd);
  set_underscore_cursor(lcd);
  move_cursor_to_field(lcd, 0);
  redraw_all_fields();
}

void loop() {
  update_all_buttons();

  if (field_is_selected()) {
    field_t * selected_field = get_selected_field();

    if (button_pressed(&increment)) {
      float increment_by = get_increment_value(selected_field);

      mutate_field(selected_field, increment_by);
    } else if (button_pressed(&decrement)) {
      float decrement_by = -1 * get_increment_value(selected_field);

      mutate_field(selected_field, decrement_by);
    } else if (button_pressed(&select)) {
      Serial.println("**************************deselected pressed");
      if (selected_field->type == TYPE_NUMERICAL) {
        save_pid_value(get_eeprom_address(), selected_field->value);
        set_pidbox_sentinel();
      } else if (selected_field->type == TYPE_STATUS) {
        selected_field->value = STATE_WAITING;
        status_is_dirty = 1;
        transmit_pid_values();
      }

      deselect_field(selected_field);
    }
  }

  // If a field isn't selected
  else {
    field_t * highlighted_field = get_highlighted_field();

    if (button_pressed(&increment)) {
      if (status.value == STATE_ERROR || status.value == STATE_RECVD) {
        status.value = STATE_READY;
      }

      move_cursor_to_prev_field(lcd);
    } else if (button_pressed(&decrement)) {
      if (status.value == STATE_ERROR || status.value == STATE_RECVD) {
        status.value = STATE_READY;
      }

      move_cursor_to_next_field(lcd);
    } else if (button_pressed(&select)) {
      Serial.println("**************************selected pressed");
      select_field(highlighted_field);

      if (highlighted_field->type == TYPE_STATUS) {
        switch ((int) highlighted_field->value) {
          case STATE_READY:
            select_field(highlighted_field);
            break;
          case STATE_WAITING:
            uint8_t values_were_recvd = 0;

            for (int i = 0; i < MAX_TRIES; i++) {
              delay(500);

              if (read_returned_pid_values()) {
                highlighted_field->value = STATE_RECVD;
                status_is_dirty = 1;
                values_were_recvd = 1;
                break;
              }
            }

            if (values_were_recvd) {
              break;
            }

            highlighted_field->value = STATE_ERROR;
            status_is_dirty = 1;
        } // switch
      }
    }
  }

  redraw_line(get_current_field(lcd));
  // if (status_is_dirty) {
  //   redraw_line(3);
  //   status_is_dirty = 0;
  // }
}

void transmit_pid_values(void) {
  myXbee->write((char *) (&(k_proportional.value)), sizeof(float));
  myXbee->write((char *) (&(k_integral.value)), sizeof(float));
  myXbee->write((char *) (&(k_derivative.value)), sizeof(float));
}

uint8_t read_returned_pid_values(void) {
  if (myXbee->available()) {
    while (myXbee->available()) {
      for (int byte_index = 0; byte_index < sizeof(pid_buff); byte_index++) {
        pid_buff[byte_index] = myXbee->read();
      }
    }

    k_proportional.value = (float) pid_buff[0];
    k_integral.value = (float) pid_buff[sizeof(float)];
    k_derivative.value = (float) pid_buff[2 * sizeof(float)];

    return 1;
  }

  return 0;
}

float get_increment_value(field_t * field) {
  if (field->type == TYPE_NUMERICAL) {
    return get_increment_value(increment_by.value);
  }

  return 1;
}

int get_eeprom_address(void) {
  switch (get_current_field(lcd)) {
    case 0:
      return K_PROP_ADDR;
    case 1:
      return K_INTEG_ADDR;
    case 2:
      return K_DERIV_ADDR;
  }
}

void init_line_buffers(void) {
  memset(line_0, 0x20, NUM_COLUMNS);
  memset(line_1, 0x20, NUM_COLUMNS);
  memset(line_2, 0x20, NUM_COLUMNS);
  memset(line_3, 0x20, NUM_COLUMNS);
}

void set_all_fields(void) {
  all_fields[0] = &k_proportional;
  all_fields[1] = &k_integral;
  all_fields[2] = &k_derivative;
  all_fields[3] = &increment_by;
  all_fields[4] = &status;

  char val_buff[4] = {0};

  if (pidbox_sentinel_is_set()) {
    float saved_k_prop = get_pid_value(K_PROP_ADDR);
    float saved_k_integ = get_pid_value(K_INTEG_ADDR);
    float saved_k_deriv = get_pid_value(K_DERIV_ADDR);

    Serial.println("****pidbox sentinel is set****");
    init_field(&k_proportional, 0, 0, saved_k_prop);
    init_field(&k_integral, 0, 0, saved_k_integ);
    init_field(&k_derivative, 0, 0, saved_k_deriv);
  } else {
    Serial.println("****pidbox sentinel is NOT set****");
    init_field(&k_proportional, 0, 0, K_PROP_DEFAULT);
    init_field(&k_integral, 0, 0, K_INTEG_DEFAULT);
    init_field(&k_derivative, 0, 0, K_DERIV_DEFAULT);
  }

  init_field(&increment_by, 0, 1, 0);
  init_field(&status, 0, 2, 0);
}

uint8_t field_is_selected(void) {
  int num_fields = sizeof(all_fields) / sizeof(field_t *);

  for (int i = 0; i < num_fields; i++) {
    if (all_fields[i]->is_selected) {
      return 1;
    }
  }

  return 0;
}

field_t * get_highlighted_field(void) {
  int field_index = get_current_field(lcd);

  return all_fields[field_index];
}

field_t * get_selected_field(void) {
  int num_fields = sizeof(all_fields) / sizeof(field_t *);

  for (int i = 0; i < num_fields; i++) {
    if (all_fields[i]->is_selected) {
      return all_fields[i];
    }
  }

  return NULL;
}

float get_pid_value(int addr) {
  char field_buff[4];

  for (int i = 0; i < 4; i++) {
    field_buff[i] = EEPROM.read(addr + i);
  }

  float saved_value;

  memcpy(&saved_value, field_buff, 4);

  return saved_value;
}

void save_pid_value(int addr, float val) {
  for (int i = 0; i < 4; i++) {
    EEPROM[addr + i] = ((char *)&val)[i];
  }
}

uint8_t pidbox_sentinel_is_set(void) {
  return (EEPROM[PIDBOX_INDICATOR_ADDR] == PIDBOX_INDICATOR_SENTINEL);
}

void set_pidbox_sentinel(void) {
  // if (pidbox_sentinel_is_set()) {
  //   return;
  // }

  // using update() instead of write, since it checks if the value is already
  // written at that address before needlessly writing it again
  EEPROM.update(PIDBOX_INDICATOR_ADDR, PIDBOX_INDICATOR_SENTINEL);
}

void redraw_line(uint8_t field_num) {
    switch (field_num) {
      case 0:
        move_cursor_to_line(lcd, 0);

        strcpy(line_0, " P: ");

        // if (k_proportional.is_selected) {
        //   strcpy(line_0, " P: ");
        // } else {
        //   strcpy(line_0, " p: ");
        // }

        update_field_text(&k_proportional);
        strcpy(line_0 + strlen(line_0), k_proportional.text);
        write_chars(lcd, line_0, NUM_COLUMNS);
        move_cursor_to_line(lcd, 0);
        break;
      case 1:
        move_cursor_to_line(lcd, 1);

        strcpy(line_1, " I: ");

        // if (k_integral.is_selected) {
        //   strcpy(line_1, " I: ");
        // } else {
        //   strcpy(line_1, " i: ");
        // }

        update_field_text(&k_integral);
        strcpy(line_1 + strlen(line_1), k_integral.text);
        write_chars(lcd, line_1, NUM_COLUMNS);
        move_cursor_to_line(lcd, 1);
        break;
      case 2:
        move_cursor_to_line(lcd, 2);

        strcpy(line_2, " D: ");

        // if (k_derivative.is_selected) {
        //   strcpy(line_2, " D: ");
        // } else {
        //   strcpy(line_2, " d: ");
        // }

        update_field_text(&k_derivative);
        strcpy(line_2 + strlen(line_2), k_derivative.text);
        write_chars(lcd, line_2, NUM_COLUMNS);
        move_cursor_to_line(lcd, 2);
        break;
      case 3:
      case 4:
        move_cursor_to_line(lcd, 3);

        strcpy(line_3, "+/-[");

        // if (increment_by.is_selected) {
        //   strcpy(line_3, "+/-[");
        // } else {
        //   strcpy(line_3, "   [");
        // }

        update_field_text(&increment_by);
        update_field_text(&status);
        strcpy(line_3 + strlen(line_3), increment_by.text);
        strcpy(line_3 + strlen(line_3), "]   ");

        // if (status.is_selected) {
        //   strcpy(line_3 + strlen(line_3), "\b*");
        // }

        strcpy(line_3 + strlen(line_3), status.text);
        write_chars(lcd, line_3, NUM_COLUMNS);
        move_cursor_to_field(lcd,get_current_field(lcd));
    }
}

void redraw_all_fields(void) {
  redraw_line(0);
  redraw_line(1);
  redraw_line(2);
  redraw_line(3);

}
