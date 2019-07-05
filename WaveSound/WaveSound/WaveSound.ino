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

int refRate = 0;

// Declare Constants  
long RMS_count_max = 30;								// Defining how many RMS values will be collected and than passed for averaging
long RMS_clear_count_max = 10;							// ------> TODO : Make LED strip refresh rate flexible <------

// Declare Variables
long audioValue = 0;									// Defining variable for A1 input analogRead() value
long actualAudioValue = 0;							// Holds audioValue - 512
long RMS = 0;										// Defining variable to hold actualAudioValue on power of 2 /formula/
long RMS_count = 0;									// Defining counter for how many RMS values are inputed
long RMS_sum = 0;									// Defining variable to hold RMS_count_max/50/ values for RMS
long RMS_clear = 0;									// Defining variable to hold the value of sqrt(RMS_sum / RMS_count)
long RMS_clear_count = 0;							// Counting how many RMS_clear values have been colected
long RMS_clear_sum = 0;								// Holds the sum of RMS_clear values and passes them to Accumulate_RMS_Average() to get their average
long RMS_average = 0;								// Holds the average of the RMS_clear values


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
	int refRate = analogRead(analog_Input_Refresh_Rate_LED) / 25;
	RMS_clear_count_max = refRate;

}

void ReadAnalogInput()
{
	//Serial.print("---------ReadAnalogInput---------");             //Printing on COM port for debuging
	//Serial.println();												 //Printing on COM port for debuging

	audioValue = analogRead(analog_Input_Aux_Mic);					 //Setting up the analog read for Aux and Mic inputs

	//Serial.println("Audio input = " + audioValue);				//Printing on COM port for debuging

	Accumulate_Audio_Value(audioValue);
}

void Accumulate_Audio_Value(long audioValue)
{
  //Serial.print("Accumulate_Audio_Value -> audioValue = ");          //Printing on COM port for debuging
  //Serial.println(_audioValue);									  //Printing on COM port for debuging
  
	actualAudioValue = audioValue - 512;

  //Serial.print("Accumulate_Audio_Value -> audioValue - 512 = ");     //Printing on COM port for debuging
  //Serial.println(_audioValue);									   //Printing on COM port for debuging

  accumulate_RMS(actualAudioValue);
}

void accumulate_RMS(long actualAudioValue)
{
	//Serial.print("Accumulate_Audio_Value -> audioValue = ");         //Printing on COM port for debuging
	//Serial.println(_audio_value);									   //Printing on COM port for debuging
  		
	RMS = pow(actualAudioValue, 2);

	//Serial.print("accumulate_RMS -> RMS = ");        //Printing on COM port for debuging
	//Serial.println(RMS);								  //Printing on COM port for debuging
  
	RMS_count++;

	//Serial.print("accumulate_RMS -> RMS_count = ");                 //Printing on COM port for debuging
	//Serial.println(RMS_count);									  //Printing on COM port for debuging

	RMS_sum = RMS_sum + RMS;
	RMS = 0;
  
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
    RMS_count = 0;
	RMS_clear = 0;
	RMS_sum = 0;
  }
}

void Accumulate_RMS_Average(long RMS_clear)   //Accumulate_RMS_Average
{
	//Serial.print("Accumulate_RMS_Average -> _RMS_clear = ");                      //Printing on COM port for debuging
	//Serial.println(RMS_clear);

	RMS_clear_sum = RMS_clear_sum + RMS_clear;

	//Serial.print("Accumulate_RMS_Average -> RMS_clear_sum = ");                      //Printing on COM port for debuging
	//Serial.println(RMS_clear_sum);

	RMS_clear_count++;

	//Serial.print("Accumulate_RMS_Average -> RMS_clear_count = ");                      //Printing on COM port for debuging
	//Serial.println(RMS_clear_count);

		if (RMS_clear_count == RMS_clear_count_max)
		{
				//Serial.print("Accumulate_RMS_Average -> RMS_clear_count = ");                      //Printing on COM port for debuging
				//Serial.println(RMS_clear_count);

				RMS_average = RMS_clear_sum / RMS_clear_count_max;

				//Serial.print("Accumulate_RMS_Average -> RMS_average = ");                      //Printing on COM port for debuging
				//Serial.println(RMS_average);

				int NumLEDs = Calculate_Number_of_LEDS_to_Light(RMS_average);

				//Serial.print("Accumulate_RMS_Average -> NumLEDs = ");                      //Printing on COM port for debuging
				//Serial.println(NumLEDs);

				LightLEDs(NumLEDs);

				//Serial.print("Accumulate_RMS_Average -> RMS_average = ");                      //Printing on COM port for debuging
				//Serial.println(RMS_average);
				RMS_clear_sum = 0;
				RMS_clear_count = 0;
				RMS_average = 0;
				//Serial.print("Accumulate_RMS_Average -> RMS_average = ");                      //Printing on COM port for debuging
				//Serial.println(RMS_average);
		}

}

int Calculate_Number_of_LEDS_to_Light(long RMS_average)
{
	//Serial.print("Calculate_Number_of_LEDS_to_Light -> _RMS_sum = ");                      //Printing on COM port for debuging
	//Serial.println(_RMS_sum);
	//Serial.print("Calculate_Number_of_LEDS_to_Light -> MAX_NUM_LEDS = ");                      //Printing on COM port for debuging
	//Serial.println(MAX_NUM_LEDS);

	int leds_to_light = (RMS_average * MAX_NUM_LEDS)/400;

	//Serial.print("Calculate_Number_of_LEDS_to_Light -> leds_to_light = ");                      //Printing on COM port for debuging
	//Serial.println(leds_to_light);

	if (leds_to_light <= 0)
	{
		leds_to_light = !leds_to_light;
	}
	return leds_to_light;
}

// ==============Led Strip=================
void LightLEDs(int NumLEDs)
{
	//Serial.print("---------LightLEDs---------");
	

  byte red_On= 0, green_On= 0, blue_On= 255;
  byte red_Off=0, green_Off=0, blue_Off=0;
  
  for (int i = 0; i < MAX_NUM_LEDS; i++) {
	  if (i <= NumLEDs)
	  {
		  if (i >= MAX_NUM_LEDS - 3 && i < MAX_NUM_LEDS)
		  {
			  byte red_On = 255, green_On = 0, blue_On = 0;
			  setPixel(i, red_On, green_On, blue_On);
		  }
		  if (i >= MAX_NUM_LEDS - 8 && i < MAX_NUM_LEDS - 3)
		  {
			  byte red_On = 255, green_On = 211, blue_On = 0;
			  setPixel(i, red_On, green_On, blue_On);
		  }
		  if (i < MAX_NUM_LEDS - 8 && i >= MAX_NUM_LEDS - 28 )
		  {
			  byte red_On = 0, green_On = 255, blue_On = 0;
			  setPixel(i, red_On, green_On, blue_On);
		  }
		  if (i < MAX_NUM_LEDS - 28)
		  {
			  byte red_On = 0, green_On = 0, blue_On = 255;
			  setPixel(i, red_On, green_On, blue_On);
		  }
	  }
    else
      setPixel(i, red_Off, green_Off, blue_Off);
  }
	//RainbowColors_p;
	showStrip();
}

void showStrip() {
    #ifdef LED_INCLUDED 
    // FastLED
    FastLED.show();
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
