//Include MuxShield Library
#include <MuxShield.h>
#include <Wire.h>

MuxShield muxShield;

//Arrays to store old Hall Effect sensor digital values (before user makes move)
int mux1DigitalValues[16];
int mux2DigitalValues[16];
int mux3DigitalValues[16];
int megaDigitalValues[16];

//Create a large array for entire board (before user makes move)
int boardState[64];
int boardStateB[64];
int occupiedSquare;

char * squares[] = {"A8","A7","A6","A5","A4","A3","A2","A1","B8","B7","B6","B5","B4","B3","B2","B1","C8","C7","C6","C5","C4","C3","C2","C1","D8","D7","D6","D5","D4","D3","D2","D1","E8","E7","E6","E5","E4","E3","E2","E1","F8","F7","F6","F5","F4","F3","F2","F1","G8","G7","G6","G5","G4","G3","G2","G1","H8","H7","H6","H5","H4","H3","H2","H1"};
//char * squares[] = {"G8","G7","G6","G5","G4","G3","G2","G1","H8","H7","H6","H5","H4","H3","H2","H1","E8","E7","E6","E5","E4","E3","E2","E1","F8","F7","F6","F5","F4","F3","F2","F1","C8","C7","C6","C5","C4","C3","C2","C1","D8","D7","D6","D5","D4","D3","D2","D1","A8","A7","A6","A5","A4","A3","A2","A1","B8","B7","B6","B5","B4","B3","B2","B1"};


void setup()
{
  
  muxShield.setMode(1,DIGITAL_IN_PULLUP);
  muxShield.setMode(2,DIGITAL_IN_PULLUP);
  muxShield.setMode(3,DIGITAL_IN_PULLUP);
  for(int i=22; i < 38; i++){
    pinMode(i, INPUT_PULLUP);
  }
  
  Serial.begin(9600);
}

void loop()
{
  for (int i=0; i<16; i++)
  {
    mux1DigitalValues[i]=muxShield.digitalReadMS(1,i);
    mux2DigitalValues[i]=muxShield.digitalReadMS(2,i);
    mux3DigitalValues[i]=muxShield.digitalReadMS(3,i);
    megaDigitalValues[i]=digitalRead(i+22);
  }
  
  
  for (int i=0; i<16; i++){
    boardState[i] = megaDigitalValues[i];
    boardState[i+16] = mux3DigitalValues[i];
    boardState[i+32] = mux2DigitalValues[i];
    boardState[i+48] = mux1DigitalValues[i];
  }
  
  for (int i=0; i<64; i++){
    boardStateB[i] = boardState[i];
    
    if (boardStateB[i] == 0){
      occupiedSquare = i;
    }
  }
  
 
  
  
  
  Serial.println("BOARDSTATE: ");
  for (int i=0; i<8; i++){
    Serial.print(boardStateB[i]);
    Serial.print("  ");}
  Serial.println(" ");
    
  for (int i=0; i<8; i++){
    Serial.print(boardStateB[i+8]);
    Serial.print("  ");}
  Serial.println(" ");
  
  for (int i=0; i<8; i++){
    Serial.print(boardStateB[i+16]);
    Serial.print("  ");}
  Serial.println(" ");
  
  for (int i=0; i<8; i++){
    Serial.print(boardStateB[i+24]);
    Serial.print("  ");}
  Serial.println(" ");
  
  for (int i=0; i<8; i++){
    Serial.print(boardStateB[i+32]);
    Serial.print("  ");}
  Serial.println(" ");
    
  for (int i=0; i<8; i++){
    Serial.print(boardStateB[i+40]);
    Serial.print("  ");}
  Serial.println(" ");
    
  for (int i=0; i<8; i++){
    Serial.print(boardStateB[i+48]);
    Serial.print("  ");}
  Serial.println(" ");
  
  for (int i=0; i<8; i++){
    Serial.print(boardStateB[i+56]);
    Serial.print("  ");}
  Serial.println(" ");
    
  Serial.println(" ");Serial.println(" ");
  Serial.println(squares[occupiedSquare]);
  Serial.println(" ");Serial.println(" ");
  delay(2000);
    
  
}
  
  
  
  
