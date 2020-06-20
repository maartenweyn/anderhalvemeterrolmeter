/*
  VoiceDistanceMeter
  
  Measures the distance using a optical encoder, changes the status of the leds and plays a song when a certain distance (1.5m) has been covered
  June 2020
  Maarten Weyn - Weyn.Tech
  https://github.com/maartenweyn/anderhalvemeterrolmeter
*/


#include "HardwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <FastLED.h>


#define EncoderA  27
#define EncoderB  12
#define Distance  -2150

#define NUM_LEDS 5
#define DATA_PIN 13


static int lastState;
static int counter = 0;

HardwareSerial DFPlayerSerial(2); //PIN16 and 17
DFRobotDFPlayerMini myDFPlayer;
CRGB leds[NUM_LEDS];

void playSound() {
  Serial.println("Play");
  myDFPlayer.play(1);  //Play the first mp3
}
void setLeds(CRGB color) {
  for (int i = 0; i< NUM_LEDS; i++) {
    leds[i] = color;
  }
  FastLED.show();
}

void setup() {  
  pinMode(EncoderA, INPUT);
  pinMode(EncoderB, INPUT);
  
  DFPlayerSerial.begin(9600, SERIAL_8N1, 16, 17); 
  
  Serial.begin(115200);

  lastState = digitalRead(EncoderA);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  setLeds(CRGB::Blue);

  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(DFPlayerSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(20);  //Set volume value. From 0 to 30

  
  setLeds(CRGB::Red);
}



void loop() {
  static int prevCounter = 0;
  
  int aState = digitalRead(EncoderA);
  if (aState != lastState) {
    if (digitalRead(EncoderB) != aState) {
      counter++;
    } else {
      counter--;
    }

    Serial.print("Position ");
    Serial.println(counter);
    lastState = aState;

    if (counter >= Distance && prevCounter < Distance) {
      playSound();
      setLeds(CRGB::Red);
    } else if (counter <= Distance && prevCounter > Distance) {
      playSound();
      setLeds(CRGB::Green);
    }


    prevCounter = counter;
  }

}
