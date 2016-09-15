#ifndef _FIELDS_H_
#define _FIELDS_H_

#define FIELD_WIDTH 7
#define NUM_INCREMENTS 5
#define PRECISION 4

#define TYPE_NUMERICAL    0
#define TYPE_INCREMENT_BY 1
#define TYPE_STATUS       2

typedef struct {
  uint8_t is_selected;
  uint8_t type;
  float value;
  char text[FIELD_WIDTH];
} field_t;

field_t * init_field(field_t * fp, uint8_t sel, uint8_t type, float val);
void deselect_field(field_t * field);
void mutate_field(field_t * field, float by);
void mutate_increment_by(field_t * field, float to);
void mutate_status(field_t * field, float to);
void mutate_numerical(field_t * field, float by);
void mutate_text(field_t * field, float to);
void select_field(field_t * field);
void set_as_numerical_field(field_t * field);
void set_as_text_field(field_t * field);
void update_field_text(field_t * field);
void update_status_field_text(field_t * field);
float get_increment_value(float increment_index);

field_t * init_field(field_t * fp, uint8_t sel, uint8_t type, float val) {
  if (fp == NULL) {
    return NULL;
  }

  fp->is_selected = sel;
  fp->type = type;
  fp->value = val;

  return fp;
}

void deselect_field(field_t * field) {
  field->is_selected = 0;
}

void mutate_field(field_t * field, float by) {
  if (field->is_selected == 0) {
    return;
  }

  switch (field->type) {
    case TYPE_NUMERICAL:
      mutate_numerical(field, by);
      break;
    case TYPE_INCREMENT_BY:
      mutate_increment_by(field, ((int)(field->value + by + NUM_INCREMENTS)) % NUM_INCREMENTS);
      break;
    case TYPE_STATUS:
      mutate_status(field, by);
  }
}

void mutate_numerical(field_t * field, float by) {
  field->value += by;

  update_field_text(field);
}

void mutate_increment_by(field_t * field, float to) {
  field->value = to;
}

void mutate_status(field_t * field, float to) {
  field->value = to;
}

void select_field(field_t * field) {
  field->is_selected = 1;
}

void set_field_type(field_t * field, uint8_t type) {
  field->type = type;
}

void update_field_text(field_t * field) {
  switch (field->type) {
    case TYPE_NUMERICAL:
      dtostrf(field->value, 2, PRECISION, field->text);
      break;
    case TYPE_INCREMENT_BY:
      { // used to contain the scope of increment_val
        // to ensure I don't try to reference it after the switch statement
        float increment_val = get_increment_value(field->value);
        dtostrf(increment_val, 2, 4, field->text);
      }
      break;
    case TYPE_STATUS:
      update_status_field_text(field);
  }
}

void update_status_field_text(field_t * field) {
    switch ((int)field->value) {
      case 0:
        strcpy(field->text, "Ready!");
        break;
      case 1:
        strcpy(field->text, "Recvd!");
        break;
      case 2:
        strcpy(field->text, "Error");
        break;
      default:
        strcpy(field->text, "???");
    }
}

float get_increment_value(float increment_index) {
  switch ((int)increment_index) {
    case 0:
      return 1.0;
      break;
    case 1:
      return 0.1;
      break;
    case 2:
      return 0.01;
      break;
    case 3:
      return 0.001;
      break;
    case 4:
      return 0.0001;
      break;
    default:
      ;// handled below
  }

  return 1.0;
}

#endif // _FIELDS_H_
