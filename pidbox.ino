/*
 * file: pidbox.ino
 * created: 20160913
 * author(s): mr-augustine, khaosduke
 *
 */
#include "Lcd420.h"
#include "buttons.h"
#include "fields.h"

#define LCD_RX_PIN 2
#define LCD_TX_PIN 3

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

void setup() {
  Serial.begin(9600);

  delay(500);
  clear_screen(lcd);

  init_buttons();
  set_all_fields();
  set_blinking_cursor(lcd);
  move_cursor_to_field(lcd, 0);
 //move_cursor(lcd,19);

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
      deselect_field(selected_field);
    }
  }

  else {
    field_t * highlighted_field = get_highlighted_field();

    if (button_pressed(&increment)) {
      move_cursor_to_prev_field(lcd);
    } else if (button_pressed(&decrement)) {
      move_cursor_to_next_field(lcd);
    } else if (button_pressed(&select)) {
      Serial.println("**************************selected pressed");

      // highlighted_field->is_selected = 1;
      //all_fields[get_current_field(lcd)]->is_selected = 1;
      select_field(highlighted_field);
      Serial.print("get current field is --------------------->>>");
      Serial.println(get_current_field(lcd));
    }

    Serial.println(k_proportional.is_selected);
    Serial.println(k_integral.is_selected);
    Serial.println(k_derivative.is_selected);
  }


  // redraw all fields
  //redraw_all_fields();all_fields[0]
  redraw_line(get_current_field(lcd));
}

float get_increment_value(field_t * field) {
  if (field->type == TYPE_NUMERICAL) {
    return get_increment_value(increment_by.value);
  }

  return 1;
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

  init_field(&k_proportional, 0, 0, 5.14567890);
  init_field(&k_integral, 0, 0, 6.56789321);
  init_field(&k_derivative, 0, 0, 9.867530909);
  init_field(&increment_by, 0, 1, 0);
  init_field(&status, 0, 2, 0);

  Serial.print("k_prop.is_selected: ");
  Serial.println(k_proportional.is_selected);
}

uint8_t field_is_selected(void) {
  Serial.println("In field_is_selected()");
  int num_fields = sizeof(all_fields) / sizeof(field_t *);

  Serial.println("------");
  Serial.print("k_prop.is_selected: ");
  Serial.println(k_proportional.is_selected);

  for (int i = 0; i < num_fields; i++) {
    if (all_fields[i]->is_selected) {
      Serial.println(all_fields[i]->is_selected, DEC);
      Serial.println("a field was selected");
      return 1;
    }
  }

  Serial.println("no fields selected");
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

void redraw_line(uint8_t field_num) {
    switch (field_num) {
      case 0:
        move_cursor_to_line(lcd, 0);
        strcpy(line_0, " P: ");
        update_field_text(&k_proportional);
        strcpy(line_0 + strlen(line_0), k_proportional.text);
        write_chars(lcd, line_0, NUM_COLUMNS);
        move_cursor_to_line(lcd, 0);
        break;
      case 1:
        move_cursor_to_line(lcd, 1);
        strcpy(line_1, " I: ");
        update_field_text(&k_integral);
        strcpy(line_1 + strlen(line_1), k_integral.text);
        write_chars(lcd, line_1, NUM_COLUMNS);
        move_cursor_to_line(lcd, 1);
        break;
      case 2:
        move_cursor_to_line(lcd, 2);
        strcpy(line_2, " D: ");
        update_field_text(&k_derivative);
        strcpy(line_2 + strlen(line_2), k_derivative.text);
        write_chars(lcd, line_2, NUM_COLUMNS);
        move_cursor_to_line(lcd, 2);
        break;
      case 3:
      case 4:
        move_cursor_to_line(lcd, 3);
        strcpy(line_3, "+/-[");
        update_field_text(&increment_by);
        update_field_text(&status);
        strcpy(line_3 + strlen(line_3), increment_by.text);
        strcpy(line_3 + strlen(line_3), "]   ");
        strcpy(line_3 + strlen(line_3), status.text);
        write_chars(lcd, line_3, NUM_COLUMNS);
        move_cursor_to_field(lcd,get_current_field(lcd));
    }
}

void redraw_all_fields(void) {
  Serial.println("In redraw_all_fields()");
  redraw_line(0);
  redraw_line(1);
  redraw_line(2);
  redraw_line(3);

}
