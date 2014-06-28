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

const int pinMotorSelector = 3;
const int pinMotorDirection = 4;
const int pinMotorStart = 5;
const int motorStep = 200; // tempo di alimentazione dei motori

const int pinVerticalEnd = 6;
const int pinButton = 7;

// Costanti per il conmando dei motori
const int MOTOR_HORIZONTAL = 0;
const int MOTOR_VERTICAL = 1;

const int FORWARDS = 1;
const int STOP = 0;
const int BACKWARDS = -1;

bool started = false;
int buttonValue = LOW;

const int DEBUG = false;

// sensore
SolarTracker tracker(pinAnalogHorizontalSensor, pinAnalogVerticalSensor, threshold);

void setup()
{
  if(DEBUG)
  {
    Serial.begin(9600);
  }
  
  pinMode(pinMotorSelector, OUTPUT);
  pinMode(pinMotorDirection, OUTPUT);
  pinMode(pinMotorStart, OUTPUT);

  pinMode(pinVerticalEnd, INPUT);
  pinMode(pinButton, INPUT);
}

void loop()
{
  // gestione pulsante
  int newButtonValue = digitalRead(pinButton);
  if(newButtonValue != buttonValue)
  {
    if(newButtonValue == HIGH)
    {
      if(started)
      {
        stop();
      }
      else
      {
        start();
      }
    }
    buttonValue = newButtonValue;
  }
  
  if(started)
  {
    SolarTracker::Data data = tracker.track();
    move(MOTOR_HORIZONTAL, data.horizontalDirection);
    if(data.horizontalDirection == STOP)
    {
      move(MOTOR_VERTICAL, data.verticalDirection);
    }
    
    if(DEBUG)
    {
      Serial.println("Sensor value - horizontal: " + String(data.horizontalDirection) + " vertical: " + String(data.verticalDirection));
    }
  }
}

// ruota l'asse orrizzontale fino a trovare il sole
void start()
{
  if(DEBUG)
  {
    Serial.println("Starting");
  }
  
  SolarTracker::Data data = tracker.track();
  while(data.horizontalDirection == STOP && data.verticalDirection == STOP)
  {
    move(MOTOR_HORIZONTAL, FORWARDS);
    data = tracker.track();
  }
  
  if(DEBUG)
  {
    Serial.println("Started");
  }
  started = true;
}

// posizione l'asse verticale sullo 0 (finecorsa)
void stop()
{
  if(DEBUG)
  {
    Serial.println("Stopping");
  }
  
  started = false;
  while(digitalRead(pinVerticalEnd) == LOW)
  {
    move(MOTOR_VERTICAL, BACKWARDS);
  }
  
  if(DEBUG)
  {
    Serial.println("Stopped");
  }
}

// Muove il servo nella direzione voluta per il tempo indicato in motorStep
// motor:
// 0 = Horizontal
// 1 = Vertical
// direction:
// 1 = avanti
// 0 = fermo
// -1 = indietro
void move(int motor, int direction)
{
  if(direction != 0)
  {
    if(motor == MOTOR_HORIZONTAL)
    {
      digitalWrite(pinMotorSelector, LOW);
    }
    else
    {
      digitalWrite(pinMotorSelector, HIGH);
    }
    
    if(direction > 0)
    {
      digitalWrite(pinMotorDirection, HIGH);
    }
    else
    {
      digitalWrite(pinMotorDirection, LOW);
    }
    delay(5);
    digitalWrite(pinMotorStart, HIGH);
    delay(motorStep);
    digitalWrite(pinMotorStart, LOW);
    digitalWrite(pinMotorDirection, LOW);
    digitalWrite(pinMotorSelector, LOW);
  }
}
