//#include <FastLED.h>

//Analog input and initial value
int analogInput = A1;
double audioValue = 0;

//RMS values
int iRMS = 0;

// Declare Constants  
int RMS_count_max=50;

// Declare Variables
int RMS_sum = 0;
int RMS_Count=0;
int RMS_Display_value=0;

int Led_Number = 3;
  
void setup() {
  // put your setup code here, to run once:
  pinMode(A1, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

int Calculate_Number_of_LEDS_to_Light(int RMS_sum)
{
  return 3;
}

void LightLEDs(int Led_Number)
{
  
}

void RMS_accumulate(int iRMS)
{
  RMS_sum = RMS_sum + iRMS;
  RMS_Count++;
  if (RMS_Count = RMS_count_max)
  {
    RMS_sum = RMS_sum / RMS_count_max;
    Led_Number = Calculate_Number_of_LEDS_to_Light(RMS_sum);
    LightLEDs(Led_Number);

    RMS_sum = 0;
    RMS_Count = 0;
  }
}

void Accumulate_Audio_Value(int audioValue)
{
  //Accumulate...
  RMS_accumulate(iRMS);
}

void ReadAnalogInput()
{
  audioValue = analogRead(analogInput); //Reading voltage from A1 - analog pin

  Accumulate_Audio_Value(audioValue);
}
