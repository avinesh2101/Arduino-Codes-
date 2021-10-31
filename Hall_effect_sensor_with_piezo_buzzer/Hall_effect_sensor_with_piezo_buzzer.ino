/*SubarnaDas for Hacktoberfest2021 */
#include "Arduino.h"
#include "HallA1302.h"
#include "AnalogReader.h"
#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"
#include "TimerFreeTone.h"
#include "Speaker.h"


#define HALLA_PIN_VOUT  A3
#define NOKIALCD_PIN_DC 4
#define NOKIALCD_PIN_CS 2
#define NOKIALCD_PIN_RST  3
#define PIEZOSPEAKER_PIN_SIG  5

//define Nokia LCD contrast and dimentions(in pixels)
#define LCD_CONTRAST 70
#define LCD_SIZE_COL 84
#define LCD_SIZE_ROW 48

unsigned int piezoSpeakerHoorayLength          = 6;                                                      // amount of notes in melody
unsigned int piezoSpeakerHoorayMelody[]        = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5}; // list of notes. List length must match HoorayLength!
unsigned int piezoSpeakerHoorayNoteDurations[] = {8      , 8      , 8      , 4      , 8      , 4      }; // note durations; 4 = quarter note, 8 = eighth note, etc. List length must match HoorayLength!

HallA1302 hallA(HALLA_PIN_VOUT);
Adafruit_PCD8544 nokiaLcd(NOKIALCD_PIN_DC, NOKIALCD_PIN_CS, NOKIALCD_PIN_RST);
Speaker piezoSpeaker(PIEZOSPEAKER_PIN_SIG);



void setup() {
  // Setup Serial which is useful for debugging
  // Use the Serial Monitor to view printed messages
  Serial.begin(9600);
  Serial.println("start");

  //Calibrate sensor
  //hallA.calibrate();
  //Initialize Nokia instance
  nokiaLcd.begin(LCD_SIZE_COL, LCD_SIZE_ROW);
  nokiaLcd.setContrast(LCD_CONTRAST); //Adjust display contrast
}

/* This code is the main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop. */
void loop() {
  //Get Measurment from hall sensor. Depending on the magnet pole polarity the sensor will return positive or negative values.
  int hallAVal = hallA.read();
  Serial.println(hallAVal);

  //Check if the box was opened
  if (abs(hallAVal - 512) < 50)
  {
    delay(500);//wait 0.5 sec
    nokiaLcd.clearDisplay();             //Erase & clear display buffer
    nokiaLcd.setRotation(2);
    nokiaLcd.setTextColor(BLACK);        //Set text color to black, background is white by default
    nokiaLcd.setTextSize(2);             //set text size
    nokiaLcd.setTextSize(1);             //set text size
    nokiaLcd.print("  Surprise !");
    nokiaLcd.setTextSize(2);             //set text size
    nokiaLcd.drawCircle(37, 15, 3, BLACK);
    nokiaLcd.drawCircle(41, 15, 3, BLACK);
    nokiaLcd.drawRect(25, 22, 30, 20, BLACK);
    nokiaLcd.drawRect(22, 18, 36, 5, BLACK);
    nokiaLcd.drawRect(37, 22, 5, 20, BLACK);
    nokiaLcd.display();                  //display on screen
    // The Speaker will play the Hooray tune
    piezoSpeaker.playMelody(piezoSpeakerHoorayLength, piezoSpeakerHoorayMelody, piezoSpeakerHoorayNoteDurations);
    delay(10000); //wait 10sec
  }

}
