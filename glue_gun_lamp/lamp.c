#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "Button.h"

#define PIN_LED 2
#define PIN_SWITCH 3
#define PIN_POT A0

#define LEDS 128

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);
Button mode = Button(PIN_SWITCH);
bool modePressed = false;

//Physical LED map for bottom and top LED arrays

//LED Bottom (Back looking from top)
//064 049 048 033 032 017 016 001
//063 050 047 034 031 018 015 002
//062 051 046 035 030 019 014 003
//061 052 045 036 029 020 013 004
//060 053 044 037 028 021 012 005
//059 054 043 038 027 022 011 006
//058 055 042 039 026 023 010 007
//057 056 041 040 025 024 009 008
//(Front looking from top)

//LED Top (Back looking from top)
//065 080 081 096 097 112 113 128
//066 079 082 095 098 111 114 127
//067 078 083 094 099 110 115 126
//068 077 084 093 100 109 116 125
//069 076 085 092 101 108 117 124
//070 075 086 091 102 107 118 123
//071 074 087 090 103 106 119 122
//072 073 088 089 104 105 120 121
//(Front looking from top)

const uint8_t botLED[] PROGMEM = {
  64,49,48,33,32,17,16,1,
  63,50,47,34,31,18,15,2,
  62,51,46,35,30,19,14,3,
  61,52,45,36,29,20,13,4,
  60,53,44,37,28,21,12,5,
  59,54,43,38,27,22,11,6,
  58,55,42,39,26,23,10,7,
  57,56,41,40,25,24,9,8,
};

const uint8_t topLED[] PROGMEM = {
  65,80,81,96,97,112,113,128,
  66,79,82,95,98,111,114,127,
  67,78,83,94,99,110,115,126,
  68,77,84,93,100,109,116,125,
  69,76,85,92,101,108,117,124,
  70,75,86,91,102,107,118,123,
  71,74,87,90,103,106,119,122,
  72,73,88,89,104,105,120,121
};

//Storage for current values
int red = 128;
int green = 128;
int blue = 128;
int pattern = 1;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() 
{
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_SWITCH, INPUT);
  pinMode(PIN_POT, INPUT);
  
  //Pixel Strip
  Serial.println("Setup()");
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //Button callbacks
  //mode.Background(ButtonBackground);

  //Set ISR for pin change on MODE pin
  Button::PinChangeSetup(PIN_SWITCH);
}

void loop() 
{
  if (modePressed)
  {
    pattern = (pattern % 8) + 1; 
    strip.clear();
  }
  modePressed = false;
  Serial.print("Mode ");
  Serial.print(pattern, DEC);
  Serial.println();
	switch (pattern)
	{
	  case 1: colorWipe(strip.Color(255, 0, 0)); break; // Red
	  case 2: colorWipe(strip.Color(0, 255, 0)); break; // Green
	  case 3: colorWipe(strip.Color(0, 0, 255)); break; // Blue
	  case 4: theaterChase(strip.Color(127, 127, 127)); break; // White
	  case 5: rainbow(); break;
    case 6: rainbowDifference(); break;
	  case 7: rainbowCycle(); break;
    case 8: rainbowCycleDifference(); break;
	  case 9: theaterChaseRainbow(); break;
	}
	if (!modePressed)
	{
    modePressed = mode.Pressed();
	}
}

//Mode button interrupt to break out of loops etc
//PCINT1 handles pin changes for pins for A0 to A5
ISR (PCINT2_vect)
{
  modePressed = modePressed | (mode.State() == LOW);
}

void ButtonBackground(void)
{
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c) 
{
  int total = strip.numPixels() / 2;
  for(uint16_t i=0; i < total && !modePressed; i++) 
  {
    uint8_t botIndex = pgm_read_byte(&botLED[i]) - 1;
    strip.setPixelColor(botIndex, c);
    uint8_t topIndex = pgm_read_byte(&topLED[i]) - 1;
    strip.setPixelColor(topIndex, c);
    strip.show();
    delay(map(analogRead(PIN_POT), 0, 1024, 100, 0));
  }
  for(uint16_t i=total; i > 0 && !modePressed; i--) 
  {
    uint8_t botIndex = pgm_read_byte(&botLED[i]) - 1;
    strip.setPixelColor(botIndex, 0);
    uint8_t topIndex = pgm_read_byte(&topLED[i]) - 1;
    strip.setPixelColor(topIndex, 0);
    strip.show();
    delay(map(analogRead(PIN_POT), 0, 1024, 100, 0));
  }
}

void rainbow() 
{
  int total = strip.numPixels() / 2;
  for(uint16_t j=0; j < 256 && !modePressed; j++) 
  {
    for(uint16_t i=0; i < total && !modePressed; i++) 
	  {
      uint32_t c = Wheel((i+j) & 255);
      uint8_t botIndex = pgm_read_byte(&botLED[i]) - 1;
      strip.setPixelColor(botIndex, c);
      uint8_t topIndex = pgm_read_byte(&topLED[i]) - 1;
      strip.setPixelColor(topIndex, c);
    }
    strip.show();
    delay(map(analogRead(PIN_POT), 0, 1024, 40, 0));
  }
}

void rainbowDifference() 
{
  int total = strip.numPixels() / 2;
  for(uint16_t j=0; j < 256 && !modePressed; j++) 
  {
    for(uint16_t i=0; i < total && !modePressed; i++) 
    {
      uint32_t c = Wheel((i+j) & 255);
      uint8_t botIndex = pgm_read_byte(&botLED[i]) - 1;
      strip.setPixelColor(botIndex, c);
      c = Wheel((i+j+64) & 255);
      uint8_t topIndex = pgm_read_byte(&topLED[i]) - 1;
      strip.setPixelColor(topIndex, c);
    }
    strip.show();
    delay(map(analogRead(PIN_POT), 0, 1024, 40, 0));
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle() 
{
  int total = strip.numPixels() / 2;
  for(uint16_t j=0; j < 256*5 && !modePressed; j++) 
  { // 5 cycles of all colors on wheel
    for(uint16_t i=0; i < total && !modePressed; i++) 
	  {
      uint32_t c = Wheel(((i * 256 / total) + j) & 255);
      uint8_t botIndex = pgm_read_byte(&botLED[i]) - 1;
      strip.setPixelColor(botIndex, c);
      uint8_t topIndex = pgm_read_byte(&topLED[i]) - 1;
      strip.setPixelColor(topIndex, c);
    }
    strip.show();
    delay(map(analogRead(PIN_POT), 0, 1024, 40, 0));
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycleDifference() 
{
  int total = strip.numPixels() / 2;
  for(uint16_t j=0; j < 256*5 && !modePressed; j++) 
  { // 5 cycles of all colors on wheel
    for(uint16_t i=0; i < total && !modePressed; i++) 
    {
      uint32_t c = Wheel(((i * 256 / total) + j) & 255);
      uint8_t botIndex = pgm_read_byte(&botLED[i]) - 1;
      strip.setPixelColor(botIndex, c);
      c = Wheel(((i * 256 / total) + j + 64) & 255);
      uint8_t topIndex = pgm_read_byte(&topLED[i]) - 1;
      strip.setPixelColor(topIndex, c);
    }
    strip.show();
    delay(map(analogRead(PIN_POT), 0, 1024, 40, 0));
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c) 
{
  int total = strip.numPixels() / 2;
  for (int j=0; j < 10 && !modePressed; j++) 
  {  //do 10 cycles of chasing
    for (int q=0; q < 3 && !modePressed; q++) 
	  {
      for (uint16_t i=0; i < total && !modePressed; i=i+3) 
	    {
        uint8_t botIndex = pgm_read_byte(&botLED[i+q]) - 1; //turn every third pixel on
        strip.setPixelColor(botIndex, c);
        uint8_t topIndex = pgm_read_byte(&topLED[i+q]) - 1;
        strip.setPixelColor(topIndex, c);
      }
      strip.show();

      delay(map(analogRead(PIN_POT), 0, 1024, 1, 150));

      for (uint16_t i=0; i < total && !modePressed; i=i+3) 
	    {
        uint8_t botIndex = pgm_read_byte(&botLED[i+q]) - 1; ////turn every third pixel off
        strip.setPixelColor(botIndex, 0);
        uint8_t topIndex = pgm_read_byte(&topLED[i+q]) - 1;
        strip.setPixelColor(topIndex, 0);
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow() 
{
  int total = strip.numPixels() / 2;
  for (int j=0; j < 256 && !modePressed; j++) 
  {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3 && !modePressed; q++) 
    {
      for (uint16_t i=0; i < total && !modePressed; i=i+3) 
	    {
        uint32_t c = Wheel( (i+j) % 255);
        uint8_t botIndex = pgm_read_byte(&botLED[i+q]) - 1; //turn every third pixel on
        strip.setPixelColor(botIndex, c);
        uint8_t topIndex = pgm_read_byte(&topLED[i+q]) - 1;
        strip.setPixelColor(topIndex, c);
      }
      strip.show();

      delay(map(analogRead(PIN_POT), 0, 1024, 1, 150));

      for (uint16_t i=0; i < total && !modePressed; i=i+3) 
	    {
        uint8_t botIndex = pgm_read_byte(&botLED[i+q]) - 1; ////turn every third pixel off
        strip.setPixelColor(botIndex, 0);
        uint8_t topIndex = pgm_read_byte(&topLED[i+q]) - 1;
        strip.setPixelColor(topIndex, 0);
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) 
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) 
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) 
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}



//https://create.arduino.cc/projecthub/john-bradnam/hot-glue-led-matrix-lamp-42322b?ref=platform&ref_id=424_trending___&offset=42
//link to detailed article 