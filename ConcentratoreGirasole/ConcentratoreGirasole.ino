//                                            
//   #     #                    ####          
//   ##   ##   ##   #  #  ###   #   #  #   #  
//   # # # #  #  #  # #   #     #   #  ##  #  
//   #  #  #  ####  ##    ###   ####   # # #  
//   #     #  #  #  # #   #     #   #  #  ##  
//   #     #  #  #  #  #  ###   #   #  #   #  
// ___________________________________________
//
//  Concentratore Girasole
//  - Concentratore con inseguitore Solare
// -------------------------------------------
//  Release: 1.0 - 19/06/2014
// -------------------------------------------
//  Code:    Giorgio Amadei
// -------------------------------------------

#include <SolarTracker.h>

const int pinAnalogHorizontalSensor = A0;
const int pinAnalogVerticalSensor = A1;
const int threshold = 50; // soglia di inattività

const int pinHorizontalMotorDirection = 3;
const int pinHorizontalMotorStart = 4;
const int pinVerticalMotorDirection = 5;
const int pinVerticalMotorStart = 6;
const int motorStep = 100; // tempo di alimentazione dei motori

// sensore 
SolarTracker tracker(pinAnalogHorizontalSensor, pinAnalogVerticalSensor, threshold);

void setup()
{ 
  pinMode(pinHorizontalMotorDirection, OUTPUT);
  pinMode(pinHorizontalMotorStart, OUTPUT); 
  pinMode(pinVerticalMotorDirection, OUTPUT);
  pinMode(pinVerticalMotorStart, OUTPUT); 
}

void loop()
{
  SolarTracker::Data data = tracker.track();
  move(pinHorizontalMotorStart, pinHorizontalMotorDirection, data.horizontalDirection);
  move(pinVerticalMotorStart, pinVerticalMotorDirection, data.verticalDirection);
}

// Muove il servo nella direzione voluta per il tempo indicato in motorStep
// direction:
//    1 = avanti
//    0 = fermo
//   -1 = indietro
void move(int pinMotorStart, int pinMotorDirection, int direction)
{
  if(direction != 0)
  {
    if(direction > 0)
    {
      digitalWrite(pinMotorDirection, HIGH);
    }
    else
    {
      digitalWrite(pinMotorDirection, LOW);
    }
    digitalWrite(pinMotorStart, HIGH);
    delay(motorStep);
    digitalWrite(pinMotorStart, LOW);
  }
}
