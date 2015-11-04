//Code written by Jordan Grider to for an Arduino Mega to control 7-segment output for a relay board
#include "PCA9685.h"

unsigned char update = 0; 	//flag set when new data recieved from ref-controller
unsigned char chipAddr = 0; //Address of chip to update
unsigned char writeValue = 0; //Value to be displayed
int chipStartPin = 0; //Pin corresponding to starting location of the display being updated at the given chip address
byte brightness;    //TODO set initial value
/*
//*** Start editing here ***

char Arduino = 'F';

//*** Stop editing here ***

byte value = Arduino - 'A';
char seg[6][4] = {
  {
    'B','A','C','D'  }
  ,{
    'I','J','K','!'  }
  ,{
    'O','P','Q','!'  }
  ,{
    'F','E','G','H'  }
  ,{
    'L','M','N','U'  }
  ,{
    'S','R','T','V'  }
};

// [arduino number][ use segment dot 0-4 , invert display 0-4 ]
byte extra[6][8] = {
  {
    0,0,0,1,0,0,0,1 }
  ,{
    0,1,1,0,0,1,0,0 }
  ,{
    0,1,1,0,0,1,0,0 }
  ,{
    0,1,0,0,0,0,0,0 }
  ,{
    0,1,1,0,0,1,0,0 }
  ,{
    1,0,1,0,1,0,0,0 }
};

//The alphabetic character corresponding to the 7-Seg attached

char seg1addr = seg[value][0]; //7-Seg on pins 22-29
char seg2addr = seg[value][1]; //7-Seg on pins 30-37
char seg3addr = seg[value][2]; //7-Seg on pins 38-45
char seg4addr = seg[value][3]; //7-Seg on pins 46-54

//Should the dot on the 7-Seg be permanently on?
boolean seg1dot = extra[value][0];
boolean seg2dot = extra[value][1];
boolean seg3dot = extra[value][2];
boolean seg4dot = extra[value][3];

//Is the 7-Seg upside down? 
boolean seg1inv = extra[value][4];
boolean seg2inv = extra[value][5];
boolean seg3inv = extra[value][6];
boolean seg4inv = extra[value][7];

//Debugger prints over USB serial all 7-Seg Values when serial data is received (works with arduino to arduino serial running)
boolean Debugger = false;

//*** Stop editing here ***


byte seven_seg_digits[11][7] = {                           
  { 
    1,1,1,1,1,1,0   }
  ,  // = 0
  { 
    0,1,1,0,0,0,0   }
  ,  // = 1
  { 
    1,1,0,1,1,0,1   }
  ,  // = 2
  { 
    1,1,1,1,0,0,1   }
  ,  // = 3
  { 
    0,1,1,0,0,1,1   }
  ,  // = 4
  { 
    1,0,1,1,0,1,1   }
  ,  // = 5
  { 
    1,0,1,1,1,1,1   }
  ,  // = 6
  { 
    1,1,1,0,0,0,0   }
  ,  // = 7
  { 
    1,1,1,1,1,1,1   }
  ,  // = 8
  { 
    1,1,1,0,0,1,1   }
  ,   // = 9
  { 
    0,0,0,0,0,0,0   } 
};

byte seven_seg_digits_inverted[11][7] = {                  
  { 
    1,1,1,1,1,1,0   }
  ,  // = 0
  { 
    0,0,0,0,1,1,0   }
  ,  // = 1
  { 
    1,1,0,1,1,0,1   }
  ,  // = 2
  { 
    1,0,0,1,1,1,1   }
  ,  // = 3
  { 
    0,0,1,0,1,1,1   }
  ,  // = 4
  { 
    1,0,1,1,0,1,1   }
  ,  // = 5
  { 
    1,1,1,1,1,0,1   }
  ,  // = 6
  { 
    0,0,0,1,1,1,0   }
  ,  // = 7
  { 
    1,1,1,1,1,1,1   }
  ,  // = 8
  { 
    0,0,1,1,1,1,1   }
  ,   // = 9
  { 
    0,0,0,0,0,0,0   } 
};

boolean seg1change = false;
boolean seg2change = false;
boolean seg3change = false;
boolean seg4change = false;

int seg1val = -1;
int seg2val = -1;
int seg3val = -1;
int seg4val = -1;
*/
int led = 22;
int inint = 0;
char incoming = 'a';
boolean serialRX = false;

char currentaddr = 'Z';


void setup() {                
  /*
  // initialize the digital pin as an output.
  for (int pin = 22; pin < 54; pin++){
    pinMode(pin, OUTPUT); 
    digitalWrite(pin, HIGH); 
  }
  if(seg1dot) {
    digitalWrite(29, LOW); 
  }
  if(seg2dot) {
    digitalWrite(37, LOW); 
  }
  if(seg3dot) {
    digitalWrite(45, LOW); 
  }
  if(seg4dot) {
    digitalWrite(53, LOW); 
  }
*/

  Wire.begin();
  Serial1.begin(9600);
  Serial.begin(9600);  // For debugging to terminal
  
  PCA9685 pwmHomeScore;
  PCA9685 pwmVisitorScore;
  PCA9685 pwmPeriod;
  PCA9685 pwmTimeMintues;
  PCA9685 pwmTimeSeconds;
  PCA9685 pwmHomePenaltyTopMinutes;
  PCA9685 pwmHomePenaltyTopSeconds;
  PCA9685 pwmHomePenaltyBottomMinutes;
  PCA9685 pwmHomePenaltyBottomSeconds;
  PCA9685 pwmAwayPenaltyTopMinutes;
  PCA9685 pwmAwayPenaltyTopSeconds;
  PCA9685 pwmAwayPenaltyBottomMinutes;
  PCA9685 pwmAwayPenaltyBottomSeconds;
  
  pwmHomeScore.begin(10001010);
  pwmVisitorScore.begin(10001000);
  pwmPeriod.begin(10011110);
  pwmTimeMintues.begin(10100000);
  pwmTimeSeconds.begin(10100010);
  pwmHomePenaltyTopMinutes.begin(10010110);
  pwmHomePenaltyTopSeconds.begin(10011000);
  pwmHomePenaltyBottomMinutes.begin(10011010);
  pwmHomePenaltyBottomSeconds.begin(10011100);
  pwmAwayPenaltyTopMinutes.begin(10001110);
  pwmAwayPenaltyTopSeconds.begin(10010000);
  pwmAwayPenaltyBottomMinutes.begin(10010010);
  pwmAwayPenaltyBottomSeconds.begin(10010100);
  
  pwmHomeScore.init();
  pwmVisitorScore.init();
  pwmPeriod.init();
  pwmTimeMintues.init();
  pwmTimeSeconds.init();
  pwmHomePenaltyTopMinutes.init();
  pwmHomePenaltyTopSeconds.init();
  pwmHomePenaltyBottomMinutes.init();
  pwmHomePenaltyBottomSeconds.init();
  pwmAwayPenaltyTopMinutes.init();
  pwmAwayPenaltyTopSeconds.init();
  pwmAwayPenaltyBottomMinutes.init();
  pwmAwayPenaltyBottomSeconds.init();
}


void loop() {
    unsigned char decodedValue = 0;
    int i, j;
	
  if (serialRX && Debugger){
	Serial.println(chipAddr);
    Serial.println(writeValue);
	Serial.println(chipPins);
    serialRX = false;
  }  
  
  //TODO handle brightness logic
  
  //Display update logic
  if (update == 1){
  	//handle horn operation
  	    //TODO
  	
  	//Decode the display value
  	decodedValue = sevenSegDecode(writeValue);
  	
  	//Write the value to the display
  	for (i = chipStartPin; i<chipStartPin + 7; i ++){
  	    j = 0;
  	    //case statemnt to determine which pwm object to apply changes to based on chipAddr
  	    switch(chipAddr){
  	        case H :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomeScore.setLEDOff(i)
  	            } else {
  	                pwmHomeScore.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case G :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomeScore.setLEDOff(i)
  	            } else {
  	                pwmHomeScore.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case B :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmVisitorScore.setLEDOff(i)
  	            } else {
  	                pwmVisitorScore.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case A :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmVisitorScore.setLEDOff(i)
  	            } else {
  	                pwmVisitorScore.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case F :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmTimeMinutes.setLEDOff(i)
  	            } else {
  	                pwmTimeMinutes.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case E :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmTimeMinutes.setLEDOff(i)
  	            } else {
  	                pwmTimeMinutes.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case D :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmTimeSeconds.setLEDOff(i)
  	            } else {
  	                pwmTimeSeconds.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case C :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmTimeSeconds.setLEDOff(i)
  	            } else {
  	                pwmTimeSeconds.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case N :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyTopMinutes.setLEDOff(i)
  	            } else {
  	                pwmHomePenaltyTopMinutes.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case M :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyTopSeconds.setLEDOff(i)
  	            } else {
  	                pwmHomePenaltyTopSeconds.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case L :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyTopSeconds.setLEDOff(i)
  	            } else {
  	                pwmHomePenaltyTopSeconds.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case T :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyBottomMinutes.setLEDOff(i)
  	            } else {
  	                pwmHomePenaltyBottomMinutes.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case S :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyBottomSeconds.setLEDOff(i)
  	            } else {
  	                pwmHomePenaltyBottomSeconds.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case R :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyBottomSeconds.setLEDOff(i)
  	            } else {
  	                pwmHomePenaltyBottomSeconds.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case K :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyTopMinutes.setLEDOff(i)
  	            } else {
  	                pwmAwayPenaltyTopMinutes.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case J :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyTopSeconds.setLEDOff(i)
  	            } else {
  	                pwmAwayPenaltyTopSeconds.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case I :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyTopSeconds.setLEDOff(i)
  	            } else {
  	                pwmAwayPenaltyTopSeconds.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case Q :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyBottomMinutes.setLEDOff(i)
  	            } else {
  	                pwmAwayPenaltyBottomMinutes.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case P :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyBottomSeconds.setLEDOff(i)
  	            } else {
  	                pwmAwayPenaltyBottomSeconds.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case O :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyBottomSeconds.setLEDOff(i)
  	            } else {
  	                pwmAwayPenaltyBottomSeconds.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	        case U :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmPeriod.setLEDOff(i)
  	            } else {
  	                pwmPeriod.setLEDDimmed(i, brightness);
  	            }
  	            break;
  	    }   //end case
  	    j ++;
  	} // end for
  	
  	//Illuminate dot if neccessary. Value > 0 : illuminate. if < 0 ignore
  	    //TODO
  	
  	update = 0;	//clear the flag, wait for next update
  } //end update block

  
}   //end loop

unsigned char sevenSegDecode(unsigned char number){
	//TODO
	//Check with matt about different display mappings (see display.xlsx)
	return 0;
}

/*
void sevenSegWrite(int number, byte pin) {
  if(number == -1){
    number = 10;
  }
  byte digit = number;

  for (byte segCount = 0; segCount < 7; ++segCount) {
    digitalWrite(pin, !seven_seg_digits[digit][segCount]);
    ++pin;
  }
}

void sevenSegWriteInverted(int number, byte pin) {
  if(number == -1){
    number = 10;
  }
  byte digit = number;

  for (byte segCount = 0; segCount < 7; ++segCount) {
    digitalWrite(pin, !seven_seg_digits_inverted[digit][segCount]);
    ++pin;
  }
}
*/

void serialEvent1() {
  while (Serial1.available()) {
    char inChar = (char)Serial1.read();
    //Serial1.print(inChar);

    int number = inChar - '0';
    if (inChar == '/'){
      number = -1;
    }

	// If a number, display. Else set it as address.
    if (number < 10 && number >= -1){
	  switch(currentaddr){
		//Home Score 10s
	    case H:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  
		  break;
		//Home Score 1s
		case G:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-14, DOT on 15
		  
		  break;
		//Away Score 10s
		case B:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  
		  break;
		//Away Score 1s
		case A:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-14	DOT on 15
		  
		  break;
		//Time Minutes 10s
		case F:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  
		  break;
		//Time Minutes 1s
		case E:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  
		  break;
		//Time Seconds 10s
		case D:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  
		  break;
		//Time Seconds 1s
		case C:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  
		  break;
		//Home Penalty Top Minutes 1s
		case N:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  
		  break;
		//Home Penalty Top Seconds 10s
		case M:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  
		  break;
		//Home Penalty Top Seconds 1s
		case L:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-15
		  
		  break;
		//Home Penalty Bottom Minutes 1s
		case T:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  
		  break;
		//Home Penalty Bottom Seconds 10s
		case S:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  
		  break;
		//Home Penalty Bottom Seconds 1s
		case R:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-15
		  
		  break;
		//Away Penalty Top Minutes 1s
		case K:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  
		  break;
		//Away Penalty Top Seconds 10s
		case J:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  
		  break;
		//Away Penalty Top Seconds 1s
		case I:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-15
		  
		  break;
		//Away Penalty Bottom Minutes 1s
		case Q:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  
		  break;
		//Away Penalty Bottom Seconds 10s
		case P:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  
		  break;
		//Away Penalty Bottom Seconds 1s
		case O:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  
		  break;
		//Period
		case U:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  
		  break;
		//Horn
		case V:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipPins;
		  
		  break;
	  }
	  
      currentaddr = 'Z';
      update = 1;
    }

    currentaddr = inChar;

  }
  serialRX = true;
}

