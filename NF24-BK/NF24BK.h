const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };

#include "RF24.h"

#define BK_BTN_PRESSED 1
#define BK_LIGHT_ON 2
#define BK_LIGHT_OFF 3

struct dataScruct {
  byte device;
  byte cmd;
} bk_msg;

void initRadio(RF24 radio) {
  radio.begin();
}

void setPipes(RF24 radio, uint64_t write, uint64_t read) {
  radio.openWritingPipe(write);
  radio.openReadingPipe(1, read);
}
