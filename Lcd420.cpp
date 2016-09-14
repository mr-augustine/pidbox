#include "Lcd420.h"

Lcd420::Lcd420(int lcd_tx_pin, int lcd_rx_pin) {
  myLcd = new SoftwareSerial(lcd_tx_pin, lcd_rx_pin);
  myLcd->begin(9600);
}

Lcd420::~Lcd420() {
  delete myLcd;
}

void Lcd420::clearScreen() {
  // position the cursor to the start of the first line
  myLcd->write(254);
  myLcd->write(128);

  for (int i = 0; i < 4; i++) {
    myLcd->write("                    ");
  }
}

CLcd420 new_CLcd420(int lcd_tx_pin, int lcd_rx_pin) {
  return reinterpret_cast<void*>(new Lcd420(lcd_tx_pin, lcd_rx_pin));
}

void del_CLcd420(CLcd420 lcd420) {
  delete reinterpret_cast<CLcd420*>(lcd420);
}

void clear_screen(CLcd420 lcd420) {
  reinterpret_cast<Lcd420*>(lcd420)->clearScreen();
}
