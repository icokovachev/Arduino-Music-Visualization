//#define LED_INCLUDED 

#ifdef LED_INCLUDED 
   #include <FastLED.h>
#endif 

//Analog input and initial value
int analogInput = A1;
double audioValue = 0;

//RMS values
int iRMS = 0;

// Declare Constants  
int RMS_count_max=50;

// Declare Variables
int RMS_sum = 0;
int RMS_count = 0;
int RMS_Display_value = 0;

double temp_sum_value = 0;
double temp_RMS_sum = 0;
double calculated_RMS = 0;
int RMS_average_count = 0;
int RMS_average_count_max = 20;
double temp_RMS_average = 0;
double RMS_average = 0;

//defining for the led strip
#define MAX_NUM_LEDS 30
#define PIN 8 // ??

#ifdef LED_INCLUDED 
    CRGB leds[NUM_LEDS];
#else
    int leds[MAX_NUM_LEDS];//fix
#endif

int Led_Number = 3; //to delete
  
void setup() {
  // put your setup code here, to run once:
  pinMode(A1, INPUT); //?????
  #ifdef LED_INCLUDED 
    FastLED.addLeds<WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  #endif
}

void loop() {
  // put your main code here, to run repeatedly:
  ReadAnalogInput();
}

void Accumulate_Audio_Value(int audioValue)
{
  audioValue -= 512;
  accumulate_RMS(audioValue);
}

void accumulate_RMS(int audio_value)
{
  temp_sum_value = sq(audio_value - 512);
  RMS_count++;
  temp_RMS_sum = temp_RMS_sum + temp_sum_value;
  if (RMS_count == RMS_count_max)
  {
    calculated_RMS = sqrt(temp_RMS_sum / RMS_count);
    Accumulate_RMS_Average(calculated_RMS);

    RMS_count = 0;
    temp_sum_value = 0;
    calculated_RMS = 0;
  }
}

void Accumulate_RMS_Average(double calculated_RMS)
{
  //Accumulate_RMS_Average
  temp_RMS_average = temp_RMS_average + calculated_RMS;
  RMS_average_count++;
  if (RMS_average_count = RMS_average_count_max)
  {
    RMS_average = sqrt(temp_RMS_average / RMS_average_count);
    int NumLEDs = Calculate_Number_of_LEDS_to_Light(RMS_average);
    LightLEDs(NumLEDs);

    temp_RMS_average = 0;
    RMS_average_count = 0;
  }
}

int Calculate_Number_of_LEDS_to_Light(int RMS_sum)
{
  return 3;
}

void ReadAnalogInput()
{
  audioValue = analogRead(analogInput); //Reading voltage from A1 - analog pin
  Accumulate_Audio_Value(audioValue);
}

// ==============Led Strip=================
void LightLEDs(int Led_Number)
{
  byte red_On=0, green_On=255, blue_On=0;
  byte red_Off=0, green_Off=0, blue_Off=0;
  
  for (int i = 0; i < MAX_NUM_LEDS; i++) {
    if (i<=Led_Number)
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

void setPixel(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H 
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H 
  #ifdef LED_INCLUDED 
    // FastLED
    leds[Pixel].r = red;
    leds[Pixel].g = green;
    leds[Pixel].b = blue;
  #endif
#endif
}
