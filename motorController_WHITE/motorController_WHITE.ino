#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_StepperMotor *xAxisMotor = AFMS.getStepper(200,1);
Adafruit_StepperMotor *yAxisMotor = AFMS.getStepper(200,2);

//WRAPPERS FOR THE MOTORS
void forwardstep1() {
  xAxisMotor->onestep(FORWARD, DOUBLE);
}
void backwardstep1() {
  xAxisMotor->onestep(BACKWARD, DOUBLE);
}

void forwardstep2() {
  yAxisMotor->onestep(FORWARD, DOUBLE);
}
void backwardstep2() {
  yAxisMotor->onestep(BACKWARD, DOUBLE);
}

//WRAP THE 2 STEPPERS IN AN ACCELSTEPPER OBJECT
AccelStepper xMotor(forwardstep1, backwardstep1);
AccelStepper yMotor(forwardstep2, backwardstep2);



//char * squares[] = {"A1","B1","C1","D1","E1","F1","G1","H1","A2","B2","C2","D2","E2","F2","G2","H2","A3","B3","C3","D3","E3","F3","G3","H3","A4","B4","C4","D4","E4","F4","G4","H4","A5","B5","C5","D5","E5","F5","G5","H5","A6","B6","C6","D6","E6","F6","G6","H6","A7","B7","C7","D7","E7","F7","G7","H7","A8","B8","C8","D8","E8","F8","G8","H8"};
char * squares[] = {"G8","G7","G6","G5","G4","G3","G2","G1","H8","H7","H6","H5","H4","H3","H2","H1","E8","E7","E6","E5","E4","E3","E2","E1","F8","F7","F6","F5","F4","F3","F2","F1","C8","C7","C6","C5","C4","C3","C2","C1","D8","D7","D6","D5","D4","D3","D2","D1","A8","A7","A6","A5","A4","A3","A2","A1","B8","B7","B6","B5","B4","B3","B2","B1"};

char letters[] = {'A','B','C','D','E','F','G','H'};
char numbers[] = {'1','2','3','4','5','6','7','8'};


//float stepNumDec = boardWidth / (gearD * pi) * stepsPerRev; //total steps for the length of movable area
//int stepPerSquare = stepNumDec / 8; //steps per square, and rounds value(float --> int)

//COORDINATES
int lag = 20;
byte xButtonValue = 0;
byte yButtonValue = 0;
const int xButton = 2;
const int yButton = 4;
const int magnet = 5;

int stepPerInch = 158;
int boardLength = 14;
int stepPerSquare = 280;
int halfSquare = 140;

int xHome = -462;
int yHome = -153;
int xOld;
int yOld;
int xNew;
int yNew;
int xLine;
int yLine;
int xGraveyard = -280;


int newPosition;
int oldPosition;
int zeroPosition;
byte pieceTaken;
byte checkState;
boolean moveDirect;
int emptySquareVal;
int occupiedSquareVal;
boolean wait;


void setup(){
  AFMS.begin(); // Start the bottom shield
  Wire.begin(5);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  
  pinMode(xButton, INPUT_PULLUP);
  pinMode(yButton, INPUT_PULLUP);
  pinMode(magnet, OUTPUT);
  
  
  xMotor.setMaxSpeed(300.0);
  xMotor.setAcceleration(300.0);
  
  yMotor.setMaxSpeed(300.0);
  yMotor.setAcceleration(300.0);
  delay(6000);
  
  Serial.println("begin homing");
  homeMotors();
  
  Serial.println("Homing complete, moving to 0,0");
  moveMotors(0,0);
}


void loop(){
  
  //WAIT FOR PIECE MOVEMENT INFORMATION FROM MEGA
  Serial.println("Waiting for MEGA");
  wait = true;
  while (wait == true){delay(100);
  Serial.println("waiting");
  }
  
  Serial.println("back from i2c");
  
  Serial.print("pieceTaken: ");
  Serial.println(pieceTaken);
  Serial.print("moveDirect: ");
  Serial.println(moveDirect);
  Serial.print("emptySquareVal: ");
  Serial.println(emptySquareVal);
  Serial.print("occupiedSquareVal: ");
  Serial.println(occupiedSquareVal);
  
  
  //CONVERT THE "A1" FORM OF THE SQUARES TO STEP COORDINATES
  String emptySquareStr = squares[emptySquareVal];
  String occupiedSquareStr = squares[occupiedSquareVal];
  
  byte emptyLetterIndex;
  byte occupiedLetterIndex;
  byte emptyNumberIndex;
  byte occupiedNumberIndex;
  
  Serial.println(emptySquareStr);
  Serial.println(occupiedSquareStr);
  
  for (int i=0; i<8; i++){
    if (emptySquareStr[0] == letters[i]){
      emptyLetterIndex = i;
    }
  }
  for (int i=0; i<8; i++){
    if (occupiedSquareStr[0] == letters[i]){
      occupiedLetterIndex = i;
    }
  }
  for (int i=0; i<8; i++){
    if (emptySquareStr[1] == numbers[i]){
      emptyNumberIndex = i;
    }
  }
  for (int i=0; i<8; i++){
    if (occupiedSquareStr[1] == numbers[i]){
      occupiedNumberIndex = i;
    }
  }
    
  byte diagonalMove = 0;
    
  if (abs(emptyLetterIndex-occupiedLetterIndex) == abs(emptyNumberIndex-occupiedNumberIndex)){
   diagonalMove = 1 ;
  }
      
  
  for (int i=0; i<8; i++){
    if (emptySquareStr[0] == letters[i]){
      xOld = (i)*stepPerSquare + halfSquare;
    }
  }
  
  for (int i=0; i<8; i++){
    if (emptySquareStr[1] == numbers[i]){
      yOld = (i)*stepPerSquare + halfSquare;
    }
  }
  
  for (int i=0; i<8; i++){
    if (occupiedSquareStr[0] == letters[i]){
      xNew = (i)*stepPerSquare + halfSquare;
    }
  }
  
  for (int i=0; i<8; i++){
    if (occupiedSquareStr[1] == numbers[i]){
      yNew = (i)*stepPerSquare + halfSquare;
    }
  }
 

  
 
  if (pieceTaken == 1){
    //MOVE MOTORS TO CUT PIECE CORDINATE 'NEW'
    xMotor.setMaxSpeed(300.0);
    xMotor.setAcceleration(300.0);
  
    yMotor.setMaxSpeed(300.0);
    yMotor.setAcceleration(300.0);
    
    moveMotorsDual(xNew,yNew);
    delay(500);
    
    xMotor.setMaxSpeed(170.0);
    xMotor.setAcceleration(50.0);
    yMotor.setMaxSpeed(170.0);
    yMotor.setAcceleration(50.0);
    
    //TURN ON ELECTROMAGNET
    digitalWrite(magnet, HIGH);
    delay(500);
    
    //MOVE MOTORS TO GRAVEYARD
    //when piece is cut, always act as though moveDirect == true
    yLine = yNew + halfSquare;
    moveMotors(xNew,yLine);
    moveMotors(xGraveyard,yLine);
    
    //TURN OFF E.M.
    digitalWrite(magnet, LOW);
    delay(500);
    
    //MOVE MOTORS TO 'OLD'
    moveMotorsDual(xOld, yOld);
    
    //TURN ON E.M.
    digitalWrite(magnet, HIGH);
    delay(500);
    
    //MOVE MOTORS TO 'NEW'
    xLine = xOld + halfSquare;
    moveMotors(xLine,yOld);
    moveMotors(xLine,yLine);
    moveMotors(xNew,yLine);
    moveMotors(xNew,yNew);
    
    //TURN OFF E.M.
    digitalWrite(magnet, LOW);
    delay(500);
  }
  
  else {
    //MOVE MOTORS TO 'OLD'
    xMotor.setMaxSpeed(400.0);
    xMotor.setAcceleration(300.0);
  
    yMotor.setMaxSpeed(400.0);
    yMotor.setAcceleration(300.0);
    
    moveMotorsDual(xOld,yOld);
    
    //TURN ON E.M.
    digitalWrite(magnet, HIGH);
    delay(500);
    
    xMotor.setMaxSpeed(170.0);
    xMotor.setAcceleration(50.0);
    yMotor.setMaxSpeed(170.0);
    yMotor.setAcceleration(50.0);
    
    //MOVE MOTORS TO 'NEW'
    if (moveDirect == false){
      if (diagonalMove == 1){
        moveMotorsDual(xNew,yNew);
      }
      
      else {
        xLine = xOld + halfSquare;
        yLine = yNew + halfSquare;
        moveMotors(xLine,yOld);
        moveMotors(xLine,yLine);
        moveMotors(xNew,yLine);
        moveMotors(xNew,yNew);
      }
    }
    else {
      moveMotors(xNew,yNew); 
    }
    
    //TURN OFF E.M.
    digitalWrite(magnet, LOW);
    delay(500);
    
  }
  
  byte moveDone = 1;
  Wire.beginTransmission(4);
  Wire.write(moveDone);
  Wire.endTransmission();
  
  //MOVE MOTORS TO 'ZERO'
  //moveMotors(0,0);
}


void receiveEvent(int howMany) {
  
  Serial.println("Receiving Data");
  
  while (1 < Wire.available()){
    while (2 < Wire.available()){
      while (3 < Wire.available()){
        pieceTaken = Wire.read();
      }
      moveDirect = Wire.read();
    }
    emptySquareVal = Wire.read();
  }
  occupiedSquareVal = Wire.read();
  
  wait = false;
 
}



void moveMotors(int xPosition, int yPosition){
  
   xMotor.moveTo(xPosition);
   yMotor.moveTo(yPosition);
  
   while(xMotor.distanceToGo() != 0)
   {
      xMotor.run();
   }
  
   while(yMotor.distanceToGo() != 0)
   {
      yMotor.run();
   }
}

void moveMotorsDual(int xPosition, int yPosition){
  
   xMotor.moveTo(xPosition);
   yMotor.moveTo(yPosition);
  
   while(xMotor.distanceToGo() != 0 || yMotor.distanceToGo() != 0)
   {
      xMotor.run();
      yMotor.run();
   }
}

void homeMotors(){
  xButtonValue = digitalRead(xButton);
  Serial.println(xButtonValue);
  while(xButtonValue == LOW){
    //move until it hits the switch and stops
    xMotor.moveTo(-5000);
    xMotor.run();
    xButtonValue = digitalRead(xButton);
  }
  xMotor.setCurrentPosition(xHome);
  
  yButtonValue = digitalRead(yButton);
  Serial.println(yButtonValue);
  while(yButtonValue == LOW){
    //move until it hits the switch and stops
    yMotor.moveTo(-5000);
    yMotor.run();
    yButtonValue = digitalRead(yButton);
  }
  yMotor.setCurrentPosition(yHome);
}
  
  
  
  
  
  
