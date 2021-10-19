// Using Arduino Uno
// DAC R-2R Ladder 8 bit tutorials and schematic can be seen at

void setup()
{
  Serial.begin(9600);
  DDRD = B11111111; // Port D at Arduino Uno (pin 0-7)
}
void loop()
{
  for ( int i = 0; i < 256; i++) { //looping the value of i from 0 to 255, creating a ramp wave from 0 to 255, it's more like stairs wave
    PORTD = i; // the integer value of i is automatically translated to binary and assign to port D, ex 3 is automatically transformed into 00000011 which means only pin 0 and pin 1 is high (1) and the other pin is low (0)
    delay(10); // set the delay of the analog value
    Serial.println(analogRead(A0)); //read the output of the DAC, translated into digital again by the ADC in analog read arduino, but we can see the plot using serial plotter without oscilloscope. But oscilloscope has the best image to show than serial plotter ADC arduino uno
  }
}