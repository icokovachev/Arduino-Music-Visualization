/*
 Name:		soundWave.ino
 Created:	6/23/2019 8:21:12 PM
 Author:	HristoKovachev
*/

#include <FastLED.h>
#include <EEPROM.h>

//defining for analog input
#define analog_Input_Aux_Mic A1
#define analog_Input_Refresh_Rate_LED A2
#define latency 50

//defining for the led strip
#define NUM_LEDS 30
#define PIN 8
CRGB leds[NUM_LEDS];

void setup() {
	FastLED.addLeds<WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}

void loop()
{
	int analogInputAuxMic = analogRead(analog_Input_Aux_Mic);
	Serial.print(analogInputAuxMic);
	Serial.println();
	int analogInputRefreshRateLED = analogRead(analog_Input_Refresh_Rate_LED);
	Serial.print(analogInputRefreshRateLED);
	Serial.println();
	delay(latency);
	if (analogInputAuxMic > 400 || analogInputAuxMic < 600 )
	{
		RGBLoop();
	}
}

void RGBLoop() 
{
	for (int j = 0; j < 3; j++) {
		// Fade IN
		for (int k = 0; k < 256; k++) {
			switch (j) {
			case 0: setAll(k, 0, 0); break;
			case 1: setAll(0, k, 0); break;
			case 2: setAll(0, 0, k); break;
			}
			showStrip();
			delay(3);
		}
		// Fade OUT
		for (int k = 255; k >= 0; k--) {
			switch (j) {
			case 0: setAll(k, 0, 0); break;
			case 1: setAll(0, k, 0); break;
			case 2: setAll(0, 0, k); break;
			}
			showStrip();
			delay(3);
		}
	}
}

void setAll(byte red, byte green, byte blue) {
	for (int i = 0; i < NUM_LEDS; i++) {
		setPixel(i, red, green, blue);
	}
	showStrip();
}


void showStrip() {
#ifdef ADAFRUIT_NEOPIXEL_H 
	// NeoPixel
	strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
	// FastLED
	FastLED.show();
#endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H 
	// NeoPixel
	strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H 
	// FastLED
	leds[Pixel].r = red;
	leds[Pixel].g = green;
	leds[Pixel].b = blue;
#endif
}