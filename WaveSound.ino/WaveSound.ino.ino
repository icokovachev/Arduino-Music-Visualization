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
int RMS_count = 0;
int RMS_Display_value = 0;

double temp_sum_value = 0;
double temp_RMS_sum = 0;
double calculated_RMS = 0;
int RMS_average_count = 0;
int RMS_average_count_max = 20;

int Led_Number = 3;
  
void setup() {
  // put your setup code here, to run once:
  pinMode(A1, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  ReadAnalogInput();
}

int Calculate_Number_of_LEDS_to_Light(int RMS_sum)
{
  return 3;
}

void LightLEDs(int Led_Number)
{
  
}

int accumulate_RMS(int audio_value)
{
  temp_sum_value = sq(audio_value - 512);
  RMS_count++;
  temp_RMS_sum = temp_RMS_sum + temp_sum_value;
  if (RSM_count = RMS_count_max)
  {
    calculated_RMS = sqrt(temp_RMS_sum / RMS_count);
    Accumulate_RMS_Average(calculated_RMS);
  }
}

void Accumulate_RMS_Average(double calculated_RMS)
{
  Accumulate_RMS_Average
  RMS_average_count++;
  //Acumulate_RMS_sum = Acomulate_RMS_sum + 
  if (RMS_average_count = RMS_average_count_max)
  {
    RMS_average = sqrt(temp_RMS_average / RMS_average_count);
  }
}

void ReadAnalogInput()
{
  audioValue = analogRead(analogInput); //Reading voltage from A1 - analog pin
  Accumulate_Audio_Value(audioValue);
}
