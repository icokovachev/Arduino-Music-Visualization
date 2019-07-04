/*
 Name:		WaveSound.ino
 Created:	7/4/2019 10:52:10 AM
 Author:	HristoKovachev
*/

#include <FastLED.h>

#define analog_Input_Aux_Mic A1 //Analog input pin defining
#define analog_Input_Refresh_Rate_LED A2 //Analog reading for refreshrate defining

#define latency 50 //latency between readings

#define NUM_LEDS 30 //Number of the leds on the strip
#define PIN 8 //Digital pin used to send data to the led strip after ADC(Analog-to-Digital Convertor)

CRGB leds[NUM_LEDS]; //Array for data transmision (check https://www.seeedstudio.com/document/pdf/WS2812B%20Datasheet.pdf 
					// Cascade method - used for data transmision/)
											

// the setup function runs once when you press reset or power the board
void setup() {
	FastLED.addLeds<WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); //Setting up the led strip /type, pin for data transfer,
																					//model for data transfer (8 bit Green, 8 bit Red, 8 bit Blue)/
}


// the loop function runs over and over again until power down or reset
void loop() {
	int analogInputAuxMic = analogRead(analog_Input_Aux_Mic); //Setting up the analog read for Aux and Mic inputs
	Serial.print(analogInputAuxMic); //Printing on COM port for debuging
	Serial.println();	//Leaving a line so the reads can be readable 
	int analogInputRefreshRateLED = analogRead(analog_Input_Refresh_Rate_LED); //Setting up the analog read for Led strip refresh rate
	Serial.print(analogInputRefreshRateLED); //Printing on COM port for debuging
	Serial.println(); //Leaving a line so the reads can be readable
	delay(latency); //waiting for 50ms for another read
}
