#define LED_INCLUDED								// Defining LED strip

#ifdef LED_INCLUDED									// If LED strip is defined
	#include <FastLED.h>							// Use FastLED.h
#endif

//defining for analog reads and timers
#define analog_Input_Aux_Mic A1						// Defining port for analogRead() for Mic and AUX input
#define analog_Input_Refresh_Rate_LED A2			// Defining port for analogRead() for LED refresh rate controller 
#define latency 50									// Defining latency between analogRead()

//defining for the led strip
#define MAX_NUM_LEDS 30								// Defining number of Pixels on the LED strip
#define PIN 8										// Defining pin for Data tranfer to the LED strip

#ifdef LED_INCLUDED									// If LED strip is defined
CRGB leds[MAX_NUM_LEDS];							// Create Array leds[] with array size = MAX_NUM_LEDS
#endif

/////////////////Look at me//////////////////////
//RMS values					
//long iRMS = 0;										
//////////////////End//////////////////////

// Declare Constants  
long RMS_count_max=50;								// Defining how many RMS values will be collected and than passed for averaging
long RMS_clear_count_max = 20;						// ------> TODO : Make LED strip refresh rate flexible <------

// Declare Variables
int audioValue = 0;									// Defining variable for A1 input analogRead() value
int actualAudioValue = 0;							// Holds audioValue - 512
long RMS = 0;										// Defining variable to hold actualAudioValue on power of 2 /formula/
long RMS_count = 0;									// Defining counter for how many RMS values are inputed
long RMS_sum = 0;									// Defining variable to hold RMS_count_max/50/ values for RMS
long RMS_clear = 0;									// Defining variable to hold the value of sqrt(RMS_sum / RMS_count)
long RMS_clear_count = 0;							// Counting how many RMS_clear values have been colected
long RMS_clear_sum = 0;								// Holds the sum of RMS_clear values and passes them to Accumulate_RMS_Average() to get their average
long RMS_average = 0;								// Holds the average of the RMS_clear values

//////////////Look at me/////////////////////
long RMS_Display_value = 0;
///////////////End///////////////////

// the setup function runs once when you press reset or power the board
void setup()
{
	FastLED.addLeds<WS2812, PIN, GRB>(leds, MAX_NUM_LEDS).setCorrection(TypicalLEDStrip);	
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	//Serial.print("--------Loop---------");                        //Printing on COM port for debuging
	//Serial.println();												//Printing on COM port for debuging
	
	ReadAnalogInput();

	//delay(50);
}

void ReadAnalogInput()
{
	//Serial.print("---------ReadAnalogInput---------");             //Printing on COM port for debuging
	//Serial.println();												 //Printing on COM port for debuging

	audioValue = analogRead(analog_Input_Aux_Mic);					 //Setting up the analog read for Aux and Mic inputs

	//Serial.println("Audio input = " + audioValue);				//Printing on COM port for debuging

	Accumulate_Audio_Value(audioValue);

	//delay(latency);
}

void Accumulate_Audio_Value(int audioValue)
{
  //Serial.print("Accumulate_Audio_Value -> audioValue = ");          //Printing on COM port for debuging
  //Serial.println(_audioValue);									  //Printing on COM port for debuging
  
	actualAudioValue = audioValue - 512;

  //Serial.print("Accumulate_Audio_Value -> audioValue - 512 = ");     //Printing on COM port for debuging
  //Serial.println(_audioValue);									   //Printing on COM port for debuging

  accumulate_RMS(actualAudioValue);
}

void accumulate_RMS(int actualAudioValue)
{
	//Serial.print("Accumulate_Audio_Value -> audioValue = ");         //Printing on COM port for debuging
	//Serial.println(_audio_value);									   //Printing on COM port for debuging

	//////////////////////////////
	//RMS = sq(_audio_value);					
	RMS = pow(actualAudioValue, 2);
	//////////////////////////////

	//Serial.print("accumulate_RMS -> RMS = ");        //Printing on COM port for debuging
	//Serial.println(RMS);								  //Printing on COM port for debuging
  
	RMS_count++;

	//Serial.print("accumulate_RMS -> RMS_count = ");                 //Printing on COM port for debuging
	//Serial.println(RMS_count);									  //Printing on COM port for debuging

	RMS_sum = RMS_sum + RMS;

	//Serial.print("accumulate_RMS -> temp_RMS_sum = ");              //Printing on COM port for debuging
	//Serial.println(RMS_sum);									  //Printing on COM port for debuging

  if (RMS_count == RMS_count_max)
  {
	  RMS_clear = sqrt(RMS_sum / RMS_count);

	//Serial.print("accumulate_RMS -> RMS_clear = ");            //Printing on COM port for debuging
	//Serial.println(calculated_RMS);
	//Serial.print("-----------------------------------------");

    Accumulate_RMS_Average(RMS_clear);

	//Reseting for new row of RMSes
	RMS = 0;
    RMS_count = 0;
	RMS_clear = 0;
  }
}

void Accumulate_RMS_Average(int _RMS_clear)   //Accumulate_RMS_Average
{
	//Serial.print("Accumulate_RMS_Average -> _RMS_clear = ");                      //Printing on COM port for debuging
	//Serial.println(_RMS_clear);

	RMS_clear_sum = RMS_clear_sum + _RMS_clear;

	//Serial.print("Accumulate_RMS_Average -> RMS_clear_sum = ");                      //Printing on COM port for debuging
	//Serial.println(RMS_clear_sum);

	RMS_clear_count++;

	//Serial.print("Accumulate_RMS_Average -> RMS_clear_count = ");                      //Printing on COM port for debuging
	//Serial.println(RMS_clear_count);

		if (RMS_clear_count == RMS_clear_count_max)
		{
				//Serial.print("Accumulate_RMS_Average -> RMS_clear_count = ");                      //Printing on COM port for debuging
				//Serial.println(RMS_clear_count);

				RMS_average = RMS_clear_sum / RMS_clear_count;

				//Serial.print("Accumulate_RMS_Average -> RMS_average = ");                      //Printing on COM port for debuging
				//Serial.println(RMS_average);

				int NumLEDs = Calculate_Number_of_LEDS_to_Light(RMS_average);

				//Serial.print("Accumulate_RMS_Average -> NumLEDs = ");                      //Printing on COM port for debuging
				//Serial.println(NumLEDs);

				LightLEDs(NumLEDs);

				RMS_clear_sum = 0;
				RMS_clear_count = 0;
		}
}

int Calculate_Number_of_LEDS_to_Light(int RMS_average)
{
	//Serial.print("Calculate_Number_of_LEDS_to_Light -> _RMS_sum = ");                      //Printing on COM port for debuging
	//Serial.println(_RMS_sum);
	//Serial.print("Calculate_Number_of_LEDS_to_Light -> MAX_NUM_LEDS = ");                      //Printing on COM port for debuging
	//Serial.println(MAX_NUM_LEDS);

	int leds_to_light = abs((RMS_average * MAX_NUM_LEDS)/1024);

	//Serial.print("Calculate_Number_of_LEDS_to_Light -> i_leds_to_light = ");                      //Printing on COM port for debuging
	//Serial.println(i_leds_to_light);
	//Serial.print("Calculate_Number_of_LEDS_to_Light -> leds_to_light = ");                      //Printing on COM port for debuging
	//Serial.println(leds_to_light);

	if (leds_to_light <= 0)
	{
		return !leds_to_light;
	}

	return leds_to_light;
}

// ==============Led Strip=================
void LightLEDs(int NumLEDs)
{
	Serial.print("---------LightLEDs---------");
	

  byte red_On=0, green_On=255, blue_On=0;
  byte red_Off=0, green_Off=0, blue_Off=0;
  
  for (int i = 0; i < MAX_NUM_LEDS; i++) {
    if (i<=NumLEDs)
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

/////////////////////////////////////////////////////////////////////////////////////////

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

/*
void Trace(char[] func_name, char[] label, double dVariable )
{
  Serial.print(func_name);                      //Printing on COM port for debuging
  Serial.print(" -> ");                      //Printing on COM port for debuging
  Serial.print(label);                      //Printing on COM port for debuging
  Serial.println(dVariable);                      //Printing on COM port for debuging
}*/

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