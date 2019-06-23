#include <FastLED.h>

//Analog input and initial value
int analogInput = A1;
double audioValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMODE(A1, input);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void analogInput(){
  audioValue = analogRead(analogInput); //Reading voltage from A1 - analog pin
}
