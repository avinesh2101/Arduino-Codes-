/*SubarnaDas For Hacktoberfest2021 */

#include <Wire.h>
#include <DS3231.h> //Library for the rtc-module from RinkyDinkElectronics.com
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2 //Temperature sensor is connected to PIN 2   
#define DHTTYPE DHT22 // Type of the sensor is DHT22 (AM2302)

DS3231  rtc(SDA, SCL); // Init the DS3231 as "rtc" using the hardware interface
LiquidCrystal_I2C lcd(0x27,16,2); // Init the display as "lcd"
DHT dht(DHTPIN, DHTTYPE); // Init the temperature sensor as "dht"

char inputButtonState; // Variable for button value

void setup() {
  rtc.begin(); // Initialize the rtc object
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight (lcd.noBacklight(); turns it off)
  dht.begin(); // Initialize the temperature sensor
  pinMode(12,INPUT); // Initialize Arduino Digital Pin 12 as input for connecting Pushbutton
  lcd.setCursor(0,0); // Set cursor to character one, row one (lcd.setCursor(character,row);)
  lcd.print("Starting... v1"); // Display characters on the LCD
  delay(2000); // Delay for 2 seconds (2000 millisec)
}

void loop() {

  inputButtonState = digitalRead(12); // Read the Pushbutton state from digital pin 12, values are either HIGH or LOW

  lcd.clear(); // Make sure that LCD is clear
  
  do{
    //Clock and Date
    lcd.setCursor(4,0);
    lcd.print(rtc.getTimeStr()); // Display time
    lcd.setCursor(1,1);
    lcd.print(rtc.getDOWStr(1)); // Display Day-of-Week
    lcd.setCursor(7,1);
    lcd.print(rtc.getDateStr(1)); // Display date    
    delay(1000); // Delay until refresh of values (1 sec)
    inputButtonState = digitalRead(12); // Read the Pushbutton state again
    
  }while(inputButtonState != HIGH); // Repeat the retrieval of time and date until button is pressed
  

  lcd.clear(); // Clear the display before new content
  
  do{
    //Temperature and Humidity
    float temp = dht.readTemperature(); // Save the temperature from the sensor in the variable "temp"
    float hum = dht.readHumidity(); // Save the humidity from the sensor in the variable "hum"

    lcd.setCursor(1,0);
    lcd.print("Temp:");
    lcd.setCursor(8,0);
    lcd.print(temp);
    lcd.setCursor(14,0);
    lcd.print("C");
    lcd.setCursor(1,1);
    lcd.print("Hum :");
    lcd.setCursor(8,1); 
    lcd.print(hum);
    lcd.setCursor(14,1);
    lcd.print("%");
    delay(2000);
    inputButtonState = digitalRead(12);
      
  }while(inputButtonState != HIGH);
}
