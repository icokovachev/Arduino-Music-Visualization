#define LED_INCLUDED 

#ifdef LED_INCLUDED 
   #include <FastLED.h>
#endif 

//Analog input and initial value
//int analogInput = A1;
#define analog_Input_Aux_Mic A1    //Analog input pin defining
#define analog_Input_Refresh_Rate_LED A2  //Analog reading for refreshrate defining
#define latency 50  //latency between readings

int audioValue = 0;

//RMS values
long iRMS = 0;

// Declare Constants  
long RMS_count_max=50;

// Declare Variables
long RMS_sum = 0;
long RMS_count = 0;
long RMS_Display_value = 0;

long temp_sum_value = 0;
long temp_RMS_sum = 0;
long calculated_RMS = 0;
long RMS_average_count = 0;
long RMS_average_count_max = 20;
long temp_RMS_average = 0;
long RMS_average = 0;

//defining for the led strip
#define MAX_NUM_LEDS 30
#define PIN 8 // ??

#ifdef LED_INCLUDED 
    CRGB leds[MAX_NUM_LEDS];
#else
    int leds[MAX_NUM_LEDS];//fix
#endif

int Led_Number = 3; //to delete



// the setup function runs once when you press reset or power the board
void setup() {
	FastLED.addLeds<WS2812, PIN, GRB>(leds, MAX_NUM_LEDS).setCorrection(TypicalLEDStrip);	//Setting up the led strip /type, pin for data transfer,
																						//model for data transfer (8 bit Green, 8 bit Red, 8 bit Blue)/
}


// the loop function runs over and over again until power down or reset
void loop() {
	  // put your main code here, to run repeatedly:
	//Serial.print("--------Loop---------");                      //Printing on COM port for debuging
	//Serial.println();                      //Printing on COM port for debuging
	ReadAnalogInput();
	//delay(50);
}

void ReadAnalogInput()
{
	//Serial.print("---------ReadAnalogInput---------");                      //Printing on COM port for debuging
	//Serial.println();                      //Printing on COM port for debuging
	audioValue = analogRead(analog_Input_Aux_Mic);         //Setting up the analog read for Aux and Mic inputs
	//Serial.println(audioValue);                      //Printing on COM port for debuging
	Accumulate_Audio_Value(audioValue);
	//delay(latency);                               //waiting for 50ms for another read

}


void Accumulate_Audio_Value(int _audioValue)
{
  //Serial.print("Accumulate_Audio_Value -> audioValue = ");                      //Printing on COM port for debuging
  //Serial.println(_audioValue);                      //Printing on COM port for debuging
  
  _audioValue = _audioValue - 512;
  //Serial.print("Accumulate_Audio_Value -> audioValue - 512 = ");                      //Printing on COM port for debuging
  //Serial.println(_audioValue);                      //Printing on COM port for debuging

  accumulate_RMS(_audioValue);
}

/*
void Trace(char[] func_name, char[] label, double dVariable )
{
  Serial.print(func_name);                      //Printing on COM port for debuging
  Serial.print(" -> ");                      //Printing on COM port for debuging
  Serial.print(label);                      //Printing on COM port for debuging
  Serial.println(dVariable);                      //Printing on COM port for debuging
}*/

void accumulate_RMS(int _audio_value)
{
	//Serial.print("Accumulate_Audio_Value -> audioValue = ");                      //Printing on COM port for debuging
	//Serial.println(_audio_value);                      //Printing on COM port for debuging

	//temp_sum_value = sq(_audio_value);
	temp_sum_value = pow(_audio_value, 2);
  //Serial.print("accumulate_RMS -> temp_sum_value [sq(audio_value - 512)] = ");                      //Printing on COM port for debuging
  //Serial.println(temp_sum_value);                      //Printing on COM port for debuging
  
  RMS_count++;
  //Serial.print("accumulate_RMS -> RMS_count = ");                      //Printing on COM port for debuging
  //Serial.println(RMS_count);                      //Printing on COM port for debuging

  temp_RMS_sum = temp_RMS_sum + temp_sum_value;
  //Serial.print("accumulate_RMS -> temp_RMS_sum = ");                      //Printing on COM port for debuging
  //Serial.println(temp_RMS_sum);                      //Printing on COM port for debuging

  if (RMS_count == RMS_count_max)
  {
    calculated_RMS = sqrt(temp_RMS_sum / RMS_count); 
	//Serial.print("accumulate_RMS -> calculated_RMS = ");                      //Printing on COM port for debuging
	//Serial.println(calculated_RMS);

	//Serial.print("-----------------------------------------");
    Accumulate_RMS_Average(calculated_RMS);

    RMS_count = 0;
    temp_sum_value = 0;
    calculated_RMS = 0;
  }
}

void Accumulate_RMS_Average(int _calculated_RMS)
{
  //Accumulate_RMS_Average

	//Serial.print("Accumulate_RMS_Average -> calculated_RMS = ");                      //Printing on COM port for debuging
	//Serial.println(_calculated_RMS);

  temp_RMS_average = temp_RMS_average + _calculated_RMS;
  //Serial.print("Accumulate_RMS_Average -> temp_RMS_average = ");                      //Printing on COM port for debuging
  //Serial.println(temp_RMS_average);

  RMS_average_count++;
  //Serial.print("Accumulate_RMS_Average -> RMS_average_count = ");                      //Printing on COM port for debuging
  //Serial.println(RMS_average_count);

  if (RMS_average_count == RMS_average_count_max)
  {
	  //Serial.print("Accumulate_RMS_Average -> RMS_average_count = ");                      //Printing on COM port for debuging
	  //Serial.println(RMS_average_count);

    RMS_average = temp_RMS_average / RMS_average_count;
	//Serial.print("Accumulate_RMS_Average -> RMS_average = ");                      //Printing on COM port for debuging
	//Serial.println(RMS_average);

    int NumLEDs = Calculate_Number_of_LEDS_to_Light(RMS_average);
	Serial.print("Accumulate_RMS_Average -> NumLEDs = ");                      //Printing on COM port for debuging
	Serial.println(NumLEDs);

    LightLEDs(NumLEDs);

    temp_RMS_average = 0;
    RMS_average_count = 0;
  }
}

int Calculate_Number_of_LEDS_to_Light(int _RMS_sum)
{
	//Serial.print("Calculate_Number_of_LEDS_to_Light -> _RMS_sum = ");                      //Printing on COM port for debuging
	//Serial.println(_RMS_sum);
	//Serial.print("Calculate_Number_of_LEDS_to_Light -> MAX_NUM_LEDS = ");                      //Printing on COM port for debuging
	//Serial.println(MAX_NUM_LEDS);
	int leds_to_light = abs((_RMS_sum * MAX_NUM_LEDS)/1024);
	//Serial.print("Calculate_Number_of_LEDS_to_Light -> i_leds_to_light = ");                      //Printing on COM port for debuging
	//Serial.println(i_leds_to_light);
	//Serial.print("Calculate_Number_of_LEDS_to_Light -> leds_to_light = ");                      //Printing on COM port for debuging
	//Serial.println(leds_to_light);
	if (leds_to_light <= 0)
	{
		return 0;
	}
	return leds_to_light;

 // return 3;
}

//void ReadAnalogInput()
//{
//	Serial.print("---------ReadAnalogInput---------");                      //Printing on COM port for debuging
//	Serial.println();                      //Printing on COM port for debuging
//
//  audioValue = analogRead(analog_Input_Aux_Mic);         //Setting up the analog read for Aux and Mic inputs
//  Serial.println(audioValue);                      //Printing on COM port for debuging
//   
//  //audioValue = analogRead(v); //Reading voltage from A1 - analog pin
//  //Accumulate_Audio_Value(audioValue);
//  delay(latency);                               //waiting for 50ms for another read
//   
//}

// ==============Led Strip=================
void LightLEDs(int _Led_Number)
{
	Serial.print("---------LightLEDs---------");
	

  byte red_On=0, green_On=255, blue_On=0;
  byte red_Off=0, green_Off=0, blue_Off=0;
  
  for (int i = 0; i < MAX_NUM_LEDS; i++) {
    if (i<=_Led_Number)
      setPixel(i, red_On, green_On, blue_On);
    else
      setPixel(i, red_Off, green_Off, blue_Off);
  }
  showStrip();
}

void showStrip() {
#ifdef ADAFRUIT_NEOPIXEL_H 
  // NeoPixel
  strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
    #ifdef LED_INCLUDED 
    // FastLED
    FastLED.show();
    #endif
#endif
}

void setPixel(int _Pixel, byte _red, byte _green, byte _blue) {
#ifdef ADAFRUIT_NEOPIXEL_H 
  // NeoPixel
  strip.setPixelColor(_Pixel, strip.Color(_red, _green, _blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H 
  #ifdef LED_INCLUDED 
    // FastLED
    leds[_Pixel].r = _red;
    leds[_Pixel].g = _green;
    leds[_Pixel].b = _blue;
  #endif
#endif
}








//void callForLEDAnimation()
//{
//	for(int j = 0; j < 256; j++) {												// Cycling through 8 bit colors /8b Red, 8b Green, 8b Blue
//    for(int i = 0; i < MAX_NUM_LEDS; i++) {											// Cycling through the array NUM_LEDS (30 leds)
//    leds[i] = Scroll((i * 256 / MAX_NUM_LEDS + j) % 256);							// For leds[i] calling Scroll method and giving it pos from ((i * 256 / NUM_LEDS + j) % 256)
//    } 
//     FastLED.show();															// Displaying the animation on the LED strip
//	 delay(1);																	// Actual display time for one input  
//  } 
//}
//
//CRGB Scroll(int pos) {															// Method, responsible for the LED strip animation
//  CRGB color (0,0,0);															// Initializing the actual color output
//  if (pos <= 85) {																// Checking if input is in range 0-85
//    color.g = 0;																/*									*/
//    color.r = ((float)pos / 85.0f) * 255.0f;									/*		Calculating the colors		*/	
//    color.b = 255 - color.r;													/*									*/
//  } else if (pos >= 86 && pos <= 170) {											// Checking if input is in range 86-170
//    color.g = ((float)(pos - 85) / 85.0f) * 255.0f;								/*									*/
//    color.r = 255 - color.g;													/*		Calculating the colors		*/
//    color.b = 0;																/*									*/
//  } else if (pos >=171 && pos <= 256) {											// Checking if input is in range 86-170
//    color.b = ((float)(pos - 170) / 85.0f) * 255.0f;							/*									*/
//    color.g = 255 - color.b;													/*		Calculating the colors		*/
//    color.r = 1;																/*									*/
//  }
//  return color;																	// returning actuial colour to the LED strip
//}