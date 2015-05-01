//Include MuxShield Library
#include <MuxShield.h>
#include <Wire.h>

MuxShield muxShield;

//Setup square characters
char * squares[] = {"A8","A7","A6","A5","A4","A3","A2","A1","B8","B7","B6","B5","B4","B3","B2","B1","C8","C7","C6","C5","C4","C3","C2","C1","D8","D7","D6","D5","D4","D3","D2","D1","E8","E7","E6","E5","E4","E3","E2","E1","F8","F7","F6","F5","F4","F3","F2","F1","G8","G7","G6","G5","G4","G3","G2","G1","H8","H7","H6","H5","H4","H3","H2","H1"};

//char * squares[] = {"G8","G7","G6","G5","G4","G3","G2","G1","H8","H7","H6","H5","H4","H3","H2","H1","E8","E7","E6","E5","E4","E3","E2","E1","F8","F7","F6","F5","F4","F3","F2","F1","C8","C7","C6","C5","C4","C3","C2","C1","D8","D7","D6","D5","D4","D3","D2","D1","A8","A7","A6","A5","A4","A3","A2","A1","B8","B7","B6","B5","B4","B3","B2","B1"};

//Buttons
const int turnCompleteButton = 43;
byte turnCompleteVal;
const int newGameButton = 39;
byte newGameVal;
const int checkButton = 41;
byte checkVal;


//Leds
const int newGameLED = 49;
const int checkLED = 47;
const int turnCompleteLED = 45;

//Check
byte checkState;
int incomingCheckState = 0;

//Taken
const int takenSensor = 53;
byte takenSensorVal;
int takenSquare;

//Arrays to store old Hall Effect sensor digital values (before user makes move)
int mux1DigitalValues[16];
int mux2DigitalValues[16];
int mux3DigitalValues[16];
int megaDigitalValues[16];

//piece taken arrays
int mux1DigitalValuesTAKEN[16];
int mux2DigitalValuesTAKEN[16];
int mux3DigitalValuesTAKEN[16];
int megaDigitalValuesTAKEN[16];

//Arrays to store new Hall Effect sensor digital values (after user makes move)
int mux1DigitalValuesNEW[16];
int mux2DigitalValuesNEW[16];
int mux3DigitalValuesNEW[16];
int megaDigitalValuesNEW[16];

//Create a large array for entire board (before user makes move)
int boardState[64];
int boardStateTmp[64];
int boardStateTAKEN[64];
int boardStateTAKENTmp[64];
int boardStateNEW[64];
int boardStateNEWTmp[64];

//Logic Variables
int emptySquareVal;
int emptySquareValB2;
int occupiedSquareVal;
int occupiedSquareValB2;
byte pieceTaken;
boolean moveDirect;
boolean firstMove;
String emptySquareStrB2;
String occupiedSquareStrB2;
String movePieceIn;
String movePieceInOld;
String movePieceInNew;

byte waitOnUno;
byte moveDone;






void setup()
{
  firstMove = true;
  
  //Setup reed switch inputs (in preparation to read inputs)
  muxShield.setMode(1,DIGITAL_IN_PULLUP);
  muxShield.setMode(2,DIGITAL_IN_PULLUP);
  muxShield.setMode(3,DIGITAL_IN_PULLUP);
  for(int i=22; i < 38; i++){
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(takenSensor, INPUT_PULLUP);
  
  //Setup button inputs
  pinMode(turnCompleteButton, INPUT_PULLUP);
  pinMode(newGameButton, INPUT_PULLUP);
  
  pinMode(checkLED, OUTPUT);
  pinMode(newGameLED, OUTPUT);
  pinMode(turnCompleteLED, OUTPUT);
  
  //Setup I2C communication to Arduino Uno
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600); // set the baud rate
  
  //turn on newGameLED 
  digitalWrite(newGameLED, HIGH);
  
  Serial.println("Waiting for game start");
  
  //Wait for newGameButton to be pushed
  newGameVal = digitalRead(newGameButton);
  while (newGameVal == LOW){
    newGameVal = digitalRead(newGameButton);
    delay(50);
  }
  
  //turn off newGameLED 
  digitalWrite(newGameLED, LOW);
  
  Serial.println("START THE GAME LETS GOOOOO BOYS");
}





void loop()
{ 
  //White will always make the first move. For the first move only, readBoard() onlyneeds to be executed.
  if (firstMove == true){
  readBoard();
  firstMove = false;
  }
  
  else {
  receiveBoard();
  readBoard();
  }
}






void receiveBoard(){
  //Wait to receive opponents move (string from the computer)
  String movePieceIn = "";
  while (!Serial.available()) {} //waiting until there is something ro read
  while (Serial.available()){ // only send data back if data has been sent
    char inByte = Serial.read(); // read the incoming data
    movePieceIn += (String) inByte;
    delay(50);
  }
  
 //movePieceIn is the string received from the computer  
 //format: A1B201
  String movePieceInOld = movePieceIn.substring(0,2);
  String movePieceInNew = movePieceIn.substring(2,4);
  String pieceTakenStr = movePieceIn.substring(4,5);
  int pieceTaken = pieceTakenStr.toInt();
  String incomingCheckStateStr = movePieceIn.substring(5);
  int incomingCheckState = incomingCheckStateStr.toInt();
 
  for (int i=0; i<64; i++)
  {
    String squaresHold = String(squares[i]);
    if (squaresHold == movePieceInOld)
    {
      emptySquareValB2 = i;
    }
    else if (squaresHold == movePieceInNew)
    {
      occupiedSquareValB2 = i;  
    }
  }

  Serial.print("emptySquareValB2: ");
  Serial.println(emptySquareValB2);
  Serial.print("emptySquareValB2: ");
  Serial.println(occupiedSquareValB2);

  //Determine whether move can be done directly without moving to lines
  if ((movePieceInOld[0]==movePieceInNew[0]) || (movePieceInOld[1]==movePieceInNew[1]))
  {
    moveDirect = true;
  }
  else {
    moveDirect = false;
  }
 
  //Output to Arduino Uno
  Wire.beginTransmission(5);
  Wire.write(pieceTaken);
  Wire.write(moveDirect);
  Wire.write(emptySquareValB2);
  Wire.write(occupiedSquareValB2);
  Wire.endTransmission();
 
  //Remain in while loop until motors stop moving
  waitOnUno = 1;
  while (waitOnUno == 1)
  {
    delay(1000);
  }
 
}






void readBoard(){
  
  //Read state of the board: read Hall Effect sensor inputs and store them in defined arrays
  for (int i=0; i<16; i++)
  {
    mux1DigitalValues[i]=muxShield.digitalReadMS(1,i);
    mux2DigitalValues[i]=muxShield.digitalReadMS(2,i);
    mux3DigitalValues[i]=muxShield.digitalReadMS(3,i);
    megaDigitalValues[i]=digitalRead(i+22);
    
    boardStateTmp[i]=megaDigitalValues[i];
    boardStateTmp[i+16]=mux3DigitalValues[i];
    boardStateTmp[i+32]=mux2DigitalValues[i];
    boardStateTmp[i+48]=mux1DigitalValues[i];
  }
  
  //fix the wiring issue
  for (int i=0; i<64; i++)
  {
    boardState[i] = boardStateTmp[63-i];
  }

  //turn turnCompleteLED on
  digitalWrite(turnCompleteLED, HIGH);

  byte mode = 0;
  
  //Wait for Turn Complete/Check/CheckMate Signal
  turnCompleteVal = digitalRead(turnCompleteButton);
  while(turnCompleteVal == LOW) 
  {
    //check turn buttons
    turnCompleteVal = digitalRead(turnCompleteButton);
    checkVal = digitalRead(checkButton);
    
    takenSensorVal = digitalRead(takenSensor);
    
    if (takenSensorVal == 0 && mode == 0)
    {
      for (int i=0; i<16; i++)
      {
        mux1DigitalValuesTAKEN[i]=muxShield.digitalReadMS(1,i);
        mux2DigitalValuesTAKEN[i]=muxShield.digitalReadMS(2,i);
        mux3DigitalValuesTAKEN[i]=muxShield.digitalReadMS(3,i);
        megaDigitalValuesTAKEN[i]=digitalRead(i+22);
        
        boardStateTAKENTmp[i]=megaDigitalValuesTAKEN[i];
        boardStateTAKENTmp[i+16]=mux3DigitalValuesTAKEN[i];
        boardStateTAKENTmp[i+32]=mux2DigitalValuesTAKEN[i];
        boardStateTAKENTmp[i+48]=mux1DigitalValuesTAKEN[i];
      }
      
      for (int i=0; i<64; i++)
      {
        boardStateTAKEN[i] = boardStateTAKENTmp[63-i];
      }
      
      for (int i=0; i<64; i++)
      {
        if (boardStateTAKEN[i] != boardState[i])
        {
          takenSquare = i;
        }
      }
      mode = 1;
    }
       
    if (checkVal == HIGH)
    {
      delay(1100);
      checkVal = digitalRead(checkButton);
      if (checkVal == HIGH)
      {
        checkState = 2;
        break;
      }
      else {
        checkState = 1;
        break;
      }
    }
    checkState = 0;
  }
  
  //turn turnCompleteLED off
  digitalWrite(turnCompleteLED, LOW);
  
  //Reads state of the board 
  for (int i=0; i<16; i++)
  {
    mux1DigitalValuesNEW[i]=muxShield.digitalReadMS(1,i);
    mux2DigitalValuesNEW[i]=muxShield.digitalReadMS(2,i);
    mux3DigitalValuesNEW[i]=muxShield.digitalReadMS(3,i);
    megaDigitalValuesNEW[i]=digitalRead(i+22);

    boardStateNEWTmp[i]=megaDigitalValuesNEW[i];
    boardStateNEWTmp[i+16]=mux3DigitalValuesNEW[i];
    boardStateNEWTmp[i+32]=mux2DigitalValuesNEW[i];
    boardStateNEWTmp[i+48]=mux1DigitalValuesNEW[i];
  }
  
  //We messed up the wire ordering, this fixes it
  for (int i=0; i<64; i++)
  {
    boardStateNEW[i] = boardStateNEWTmp[63-i];
  }
  
  //Compare boardState arrays to determine which piece was moved
  for (int i=0; i<64; i++)
  {
    if (boardStateNEW[i] != boardState[i])
    {
      if (boardStateNEW[i] == 1)
      {
        emptySquareVal = i;
      }
      
      else if (boardStateNEW[i] == 0) 
      {
        occupiedSquareVal = i;
      }
    }
  }
  
  if (mode == 1)
  {
    occupiedSquareVal = takenSquare;
    pieceTaken = 1;
  }
  
  String emptySquareStr = squares[emptySquareVal];
  String occupiedSquareStr = squares[occupiedSquareVal];
  String checkStateStr = String(checkState);
  String pieceTakenStr = String(pieceTaken);
  
  String movePiece = emptySquareStr + occupiedSquareStr + pieceTakenStr + checkStateStr;
  
  //Send move to computer 10 times to ensure entire string is sent correctly
  Serial.println(movePiece);
//  for (int i = 0; i < 10; i++) {
//    Serial.println(movePiece);
//  }
  
}

void receiveEvent(int howMany)
{
  while (Wire.available())
  {
    moveDone = Wire.read();
  }
  waitOnUno = 0;
}

