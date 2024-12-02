// Master
#include <Wire.h>
enum TrafficLightState {START,YELLOW,GREEN_WITH_PEDESTRIAN_GREEN,RED_WITH_PEDESTRIAN_GREEN};
TrafficLightState currentState = START;

void setup() {
  Wire.begin();  // Start I2C as master
  pinMode(2, INPUT);  // Button
  pinMode(3, OUTPUT); // Pedestrian Green
  pinMode(4, OUTPUT); // Pedestrian Red
  pinMode(5, OUTPUT); // D1 Green
  pinMode(6, OUTPUT); // D1 Yellow
  pinMode(7, OUTPUT); // D1 Red
  pinMode(8, OUTPUT); // D2 Green
  pinMode(9, OUTPUT); // D2 Yellow
  pinMode(10, OUTPUT); // D2 Red
  pinMode(11, OUTPUT); // D3 Green
  pinMode(12, OUTPUT); // D3 Yellow
  pinMode(13, OUTPUT); // D3 Red

  // Start_state
  digitalWrite(5, HIGH);  // D1 Green
  digitalWrite(6, LOW);   // D1 Yellow OFF
  digitalWrite(7, LOW);   // D1 Red OFF
  digitalWrite(8, HIGH);  // D2 Green
  digitalWrite(9, LOW);   // D2 Yellow OFF
  digitalWrite(10, LOW);  // D2 Red OFF
  digitalWrite(3, LOW);   // Pedestrian Green OFF
  digitalWrite(4, HIGH);  // Pedestrian Red ON
  digitalWrite(11, LOW);  // D3 Green OFF
  digitalWrite(12, LOW);  // D3 Yellow OFF
  digitalWrite(13, HIGH); // D3 Red ON

  // Send initial command to slave to set D3 to Red
  Wire.beginTransmission(8); // Send to slave with address 8
  Wire.write('R');           // Command 'R' for D3 Red
  Wire.endTransmission();
}

void loop() {
  if (digitalRead(2) == HIGH) {
    currentState = YELLOW;  // Move to yellow state when button pressed
  }

  switch (currentState) {
    case START:
      // Initial state: D1, D2 Green - Pedestrian Red, D3 Red
      break;

    case YELLOW:
      // D1, D2 to Yellow, D3 and Pedestrian stay the same
      digitalWrite(5, LOW);   // D1 Green OFF
      digitalWrite(6, HIGH);  // D1 Yellow ON
      digitalWrite(7, LOW);   // D1 Red OFF

      digitalWrite(8, LOW);   // D2 Green OFF
      digitalWrite(9, HIGH);  // D2 Yellow ON
      digitalWrite(10, LOW);  // D2 Red OFF

      digitalWrite(3, LOW);   // Pedestrian Green OFF
      digitalWrite(4, HIGH);  // Pedestrian Red ON

      Wire.beginTransmission(8); // Send to slave with address 8
      Wire.write('Y');           // Command 'Y' for D3 Yellow
      Wire.endTransmission();
      delay(2000);  

      currentState = GREEN_WITH_PEDESTRIAN_GREEN;
      break;

    case GREEN_WITH_PEDESTRIAN_GREEN:
      // D1, D2 to Red, D3 Green, Pedestrian Green
      digitalWrite(6, LOW);   // D1 Yellow OFF
      digitalWrite(7, HIGH);  // D1 Red ON
      digitalWrite(5, LOW);   // D1 Green OFF

      digitalWrite(9, LOW);   // D2 Yellow OFF
      digitalWrite(10, HIGH); // D2 Red ON
      digitalWrite(8, LOW);   // D2 Green OFF

      digitalWrite(3, HIGH);  // Pedestrian Green ON
      digitalWrite(4, LOW);   // Pedestrian Red OFF

      Wire.beginTransmission(8); // Command D3 to Green
      Wire.write('G');
      Wire.endTransmission();
      delay(5000);  

      currentState = RED_WITH_PEDESTRIAN_GREEN;
      break;

    case RED_WITH_PEDESTRIAN_GREEN:
      // Change D1, D2 to Yellow, D3 and Pedestrian stay the same
      digitalWrite(7, LOW);   // D1 Red OFF
      digitalWrite(6, HIGH);  // D1 Yellow ON
      digitalWrite(5, LOW);   // D1 Green OFF

      digitalWrite(10, LOW);  // D2 Red OFF
      digitalWrite(9, HIGH);  // D2 Yellow ON
      digitalWrite(8, LOW);   // D2 Green OFF

      digitalWrite(3, HIGH);  // Pedestrian Green ON
      digitalWrite(4, LOW);   // Pedestrian Red OFF

      Wire.beginTransmission(8); // Command D3 to Yellow
      Wire.write('Y');
      Wire.endTransmission();
      delay(2000);  

      // Reset to START
      digitalWrite(6, LOW);   // D1 Yellow OFF
      digitalWrite(5, HIGH);  // D1 Green ON

      digitalWrite(9, LOW);   // D2 Yellow OFF
      digitalWrite(8, HIGH);  // D2 Green ON

      digitalWrite(3, LOW);   // Pedestrian Green OFF
      digitalWrite(4, HIGH);  // Pedestrian Red ON

      Wire.beginTransmission(8); // Command D3 to Red
      Wire.write('R');
      Wire.endTransmission();
      delay(2000);

      currentState = START;
      break;
  }
}

//----------------------------------------------------------------------
//Slave
#include <Wire.h>

void setup() { 
    pinMode(11, OUTPUT);   // D3 Green
    pinMode(12, OUTPUT);   // D3 Yellow
    pinMode(13, OUTPUT);   // D3 Red
    Wire.begin(8);         // Start I2C as slave with address 8
    Wire.onReceive(receiveEvent); // Listen for events from master
}

void loop() { }

void receiveEvent(int bytes) {
    char state = Wire.read();
    if (state == 'G') {
        // If 'G' is received, change D3 to Green
        digitalWrite(11, HIGH);  // D3 Green ON
        digitalWrite(12, LOW);   // D3 Yellow OFF
        digitalWrite(13, LOW);   // D3 Red OFF
    }
    else if (state == 'Y') {
        // If 'Y' is received, change D3 to Yellow
        digitalWrite(11, LOW);   // D3 Green OFF
        digitalWrite(12, HIGH);  // D3 Yellow ON
        digitalWrite(13, LOW);   // D3 Red OFF
    }
    else if (state == 'R') {
        // If 'R' is received, change D3 to Red
        digitalWrite(11, LOW);   // D3 Green OFF
        digitalWrite(12, LOW);   // D3 Yellow OFF
        digitalWrite(13, HIGH);  // D3 Red ON
    }
}
