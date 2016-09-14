#ifdef __cplusplus

#include <SoftwareSerial.h>

class Lcd420 {
  public:
  Lcd420(int lcd_rx_pin, int lcd_tx_pin);
  ~Lcd420();
  void clearScreen();

  private:
  SoftwareSerial * myLcd;
};
#endif // __cplusplus

typedef void* CLcd420;

extern "C" CLcd420 new_CLcd420(int lcd_tx_pin, int lcd_rx_pin);
extern "C" void del_CLcd420(CLcd420);
extern "C" void clear_screen(CLcd420);

//SoftwareSerial * myLcd;

/*uint8_t lcd420_init(int lcd_rx_pin, int dummy_pin) {
  // myLcd = new SoftwareSerial(dummy_pin, lcd_rx_pin);

}
*/
