#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>           //  also need to include this library...
#include <SPI.h>
#include <Wire.h>

TMRpcm audio;

#define AMP_PIN 2

unsigned long time = 0;

void setup() {

  audio.speakerPin = 9;
  pinMode(AMP_PIN,OUTPUT);
  digitalWrite(AMP_PIN,LOW);

  Serial.begin(57600);

  if (SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD ok");
  } else {
    Serial.println("SD fail");
    return;
  }

  audio.stopPlayback();
  audio.setVolume(4);
  play("welcome.wav");

  Wire.begin(9);
  Wire.onReceive(receiveSound);
}

void receiveSound(int howMany) {
  int team = 0;
  
  if (!audio.isPlaying()) {
    team = Wire.read();

    switch (team) {
      case 1:
        play("team-1.wav");
        break;
      case 2:
        play("team-2.wav");
        break;
      default:
        play("welcome.wav");      
        break;
    }
  }
    
  Wire.begin(9);
}

void play(char* name) {
  digitalWrite(AMP_PIN,HIGH);
  audio.play(name);
  
  while (audio.isPlaying()) {
    delay(100);
  }
  
  digitalWrite(AMP_PIN,LOW);
}

void loop() {
  delay(100);
}
