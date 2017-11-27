#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "../../NF24-BK/NF24BK.h"

#include <Wire.h>

RF24 radio(9, 10);

#define BUTTON_PIN 5
#define LIGHT_PIN 4

byte currentSwitch = BK_SW1;

void setup() {
  Serial.begin(57600);

  printf_begin();
  printf("*** SWITCH (%2d) ***\n\r",currentSwitch);

  initRadio(radio);
  setPipes(radio, pipes[0], pipes[1]);

  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  light(true);
  delay(500);
  light(false);
  delay(500);
  
  radio.startListening();
}

long lastPress = 0;

void loop(void) {
  if (isButtonPressed() && millis()-lastPress > 100) {
    sendButtonPressed();
    radio.startListening();
    lastPress = millis();
  }

  if (radio.available()) {
    radio.read(&bk_msg, sizeof(bk_msg));
    printf("Received (DEVICE/CMD): (%2d,%2d) !\n\r",bk_msg.device,bk_msg.cmd);
    if (bk_msg.device == currentSwitch) {
      switch (bk_msg.cmd) {
        case BK_LIGHT_ON:
          light(true);
          break;
        case BK_LIGHT_OFF:
          light(false);
          break;
      }
    }
  }
}

boolean isButtonPressed() {
  return digitalRead(BUTTON_PIN) == LOW;
}

void light(boolean on) {
  digitalWrite(LIGHT_PIN, on ? HIGH : LOW);
}


boolean sendButtonPressed() {
  Serial.print("Send Button press ...");
  radio.stopListening();
  Serial.print("Stop listening ...");

  bk_msg.device = currentSwitch;
  bk_msg.cmd = BK_BTN_PRESSED;

  bool send = radio.write(&bk_msg, sizeof(bk_msg));
  Serial.print("Send: ");
  Serial.println(send);

  return send;
}
