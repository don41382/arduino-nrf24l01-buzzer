#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "../../NF24-BK/NF24BK.h"
#include <Wire.h>

RF24 radio(9, 10);

#define CLEAR_BTN_PIN 5


void setup() {
  Serial.begin(57600);

  printf_begin();
  printf("*** MASTER ***\n\r");

  initRadio(radio);
  setPipes(radio, pipes[1], pipes[0]);
  radio.startListening();

  pinMode(CLEAR_BTN_PIN, INPUT_PULLUP);

  Wire.begin();
}

long lastPress = 0;
long currentBuzzer = 0;

void loop(void) {
  if (isClearButtonPressed() && millis() - lastPress > 1000) {
    Serial.print("Clear Button Pressed: ");
    Serial.println(isClearButtonPressed());

    currentBuzzer = 0;

    radio.stopListening();
    bk_msg.device = BK_SW1;
    bk_msg.cmd = BK_LIGHT_OFF;
    radio.write(&bk_msg, sizeof(bk_msg));

    bk_msg.device = BK_SW2;
    bk_msg.cmd = BK_LIGHT_OFF;
    radio.write(&bk_msg, sizeof(bk_msg));
    radio.startListening();

    lastPress = millis();
  }

  if (radio.available()) {
    printf("Receive data ...\n\r");
    radio.read(&bk_msg, sizeof(bk_msg));

    printf("Data received! %2d, %2d\n\r", bk_msg.device, bk_msg.cmd);

    if (currentBuzzer == 0) {
      boolean send = false;
      radio.stopListening();
      delay(150);

      switch (bk_msg.cmd) {
        case BK_BTN_PRESSED:
          printf("try to send it ...\n\r");

          currentBuzzer = bk_msg.device;
          bk_msg.cmd = BK_LIGHT_ON;

          send = radio.write(&bk_msg, sizeof(bk_msg));
          sendSound(currentBuzzer);
          break;
      }

      if (send) {
        printf("Send CMD!\n\r");
      } else {
        printf("Send CMD failed!\n\r");
      }
      setPipes(radio, pipes[1], pipes[0]);
      radio.startListening();
    }
  }
  delay(50);
}

boolean isClearButtonPressed() {
  return digitalRead(CLEAR_BTN_PIN) == LOW;
}

boolean sendCmd(byte pipeNo, byte sw, byte cmd) {
  bk_msg.device = sw;
  bk_msg.cmd = BK_BTN_PRESSED;
  radio.writeAckPayload(pipeNo, &bk_msg, sizeof(bk_msg));
  return true;
  //radio.write(&bk_msg, sizeof(bk_msg));
}

void sendSound(int team) {
  Wire.beginTransmission(9);
  Wire.write(team);
  Wire.endTransmission();
  Serial.println("Send Sound CMD");
}
