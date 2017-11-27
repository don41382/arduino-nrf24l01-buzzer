#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(9, 10);

#define BUTTON_PIN 2

struct dataScruct {
  unsigned int id;
} bk_msg;

const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };

void setup() {
  printf_begin();
  printf("*** SWITCH ***\n\r");

  radio.begin();
  //radio.setAutoAck(1);                    // Ensure autoACK is enabled
  //radio.enableAckPayload();               // Allow optional ack payloads
  //radio.setRetries(0, 15);                // Smallest time between retries, max no. of retries
  //radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed
  radio.openWritingPipe(pipes[1]);        // Both radios listen on the same pipes by default, and switch when writing
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();                 // Start listening
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging

  pinMode(BUTTON_PIN, INPUT);
}

void loop(void) {
  if (isButtonPressed()) {

  }
}

boolean isButtonPressed() {
  return digitalRead(BUTTON_PIN) == HIGH;
}
