/*   
HC05 - Bluetooth AT-Command mode  
*/ 
#include <SoftwareSerial.h> 
SoftwareSerial MyBlue(2, 3); // RX | TX 
int flag = 0; 
int LED = 8; 
void setup() 
{   
  Serial.begin(9600); 
  MyBlue.begin(9600); 
  pinMode(LED, OUTPUT); 
  Serial.println("Ready to connect\nDefualt password is 1234 or 000"); 
} 
void loop() 
{ 
  if (MyBlue.available()) 
    flag = MyBlue.read(); 
  if (flag == 1) 
  { 
    digitalWrite(LED, HIGH); 
    Serial.println("LED On"); 
  } 
  else if (flag == 0) 
  { 
    digitalWrite(LED, HIGH); 
    Serial.println("LED Off"); 
  } 
} 
