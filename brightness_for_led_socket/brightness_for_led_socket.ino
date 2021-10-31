//SubarnaDas for Hacktoberfest2021
/*macro definitions of Rotary angle sensor and LED pin*/
    #define ROTARY_ANGLE_SENSOR A0
    #define LED 2//the Grove - LED is connected to D3 of Arduino
    #define ADC_REF 5//reference voltage of ADC is 5v.If the Vcc switch on the seeeduino
                     //board switches to 3V3, the ADC_REF should be 3.3
    #define GROVE_VCC 5//VCC of the grove interface is normally 5v
    #define FULL_ANGLE 300//full value of the rotary angle is 300 degrees
    void setup() 
    {
        Serial.begin(9600);
        pinsInit();
    }

    void loop() 
    {
        int degrees;
        degrees = getDegree();
        Serial.println("The angle between the mark and the starting position:");
        Serial.println(degrees);

        int brightness;
        /*The degrees is 0~300, should be converted to be 0~255 to control the*/
        /*brightness of LED                                                   */
        brightness = map(degrees, 0, FULL_ANGLE, 0, 255); 
        controlBrightness(brightness);

        delay(500);
    }
    void pinsInit()
    {
        pinMode(ROTARY_ANGLE_SENSOR, INPUT);
        pinMode(LED,OUTPUT);
    }

    /*PWM control brightness                        */
    /*If brightness is 0,the LED is off.            */
    /*The Greater the brightness, the brighter the LED.*/
    /*The range of brightness is 0~255              */
    void controlBrightness(int brightness)
    {
        analogWrite(LED,brightness);
    }
    /************************************************************************/
    /*Function: Get the angle between the mark and the starting position    */
    /*Parameter:-void                                                       */
    /*Return:   -int,the range of degrees is 0~300                          */
    int getDegree()
    {
        int sensor_value = analogRead(ROTARY_ANGLE_SENSOR);
        float voltage;
        voltage = (float)sensor_value*ADC_REF/1023;
        float degrees = (voltage*FULL_ANGLE)/GROVE_VCC;
        return degrees;
    }
