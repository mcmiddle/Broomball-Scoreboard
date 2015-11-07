//Code written by Jordan Grider to for an Arduino Mega to control 7-segment output for a relay board
#include "PCA9685.h"

unsigned char update = 0; 	//flag set when new data received from ref-controller
unsigned char chipAddr = 0; //Address of chip to update
unsigned char writeValue = 0; //Value to be displayed
char chipStartPin = 0; //Pin corresponding to starting location of the display being updated at the given chip address
byte brightness;    //TODO set initial value
char dotPin = 0;  //Pin corresponding to the dot, -1 if no dot
char decodeType = -1; //There are two different pin mappings for display depending on single digit or two digits
					  //value is used for decoding each type. 0 is one digit by itself, 1 is first digit of two, 2 is second digit of two

int led = 22;
int inint = 0;
char incoming = 'a';
boolean serialRX = false;

char currentaddr = 'Z';


void setup() {                

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
  	for (i = chipStartPin; i < chipStartPin + 7; i ++){
  	    j = 0;
  	    //case statement to determine which pwm object to apply changes to based on chipAddr
  	    switch(chipAddr){
  	        case H :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomeScore.setLEDOff(i);
  	            } else {
  	                pwmHomeScore.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmHomeScore.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case G :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomeScore.setLEDOff(i);
  	            } else {
  	                pwmHomeScore.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmHomeScore.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case B :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmVisitorScore.setLEDOff(i);
  	            } else {
  	                pwmVisitorScore.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmVisitorScore.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case A :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmVisitorScore.setLEDOff(i);
  	            } else {
  	                pwmVisitorScore.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmVisitorScore.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case F :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmTimeMinutes.setLEDOff(i);
  	            } else {
  	                pwmTimeMinutes.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmTimeMinutes.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case E :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmTimeMinutes.setLEDOff(i);
  	            } else {
  	                pwmTimeMinutes.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmTimeMinutes.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case D :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmTimeSeconds.setLEDOff(i);
  	            } else {
  	                pwmTimeSeconds.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmTimeSeconds.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case C :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmTimeSeconds.setLEDOff(i);
  	            } else {
  	                pwmTimeSeconds.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmTimeSeconds.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case N :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyTopMinutes.setLEDOff(i);
  	            } else {
  	                pwmHomePenaltyTopMinutes.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmHomePenaltyTopMinutes.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case M :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyTopSeconds.setLEDOff(i);
  	            } else {
  	                pwmHomePenaltyTopSeconds.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmHomePenaltyTopSeconds.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case L :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyTopSeconds.setLEDOff(i);
  	            } else {
  	                pwmHomePenaltyTopSeconds.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmHomePenaltyTopSeconds.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case T :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyBottomMinutes.setLEDOff(i);
  	            } else {
  	                pwmHomePenaltyBottomMinutes.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmHomePenaltyBottomMinutes.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case S :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyBottomSeconds.setLEDOff(i);
  	            } else {
  	                pwmHomePenaltyBottomSeconds.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmHomePenaltyBottomSeconds.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case R :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyBottomSeconds.setLEDOff(i);
  	            } else {
  	                pwmHomePenaltyBottomSeconds.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmHomePenaltyBottomSeconds.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case K :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyTopMinutes.setLEDOff(i);
  	            } else {
  	                pwmAwayPenaltyTopMinutes.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmAwayPenaltyTopMinutes.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case J :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyTopSeconds.setLEDOff(i);
  	            } else {
  	                pwmAwayPenaltyTopSeconds.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmAwayPenaltyTopSeconds.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case I :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyTopSeconds.setLEDOff(i);
  	            } else {
  	                pwmAwayPenaltyTopSeconds.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmAwayPenaltyTopSeconds.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case Q :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyBottomMinutes.setLEDOff(i);
  	            } else {
  	                pwmAwayPenaltyBottomMinutes.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmAwayPenaltyBottomMinutes.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case P :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyBottomSeconds.setLEDOff(i);
  	            } else {
  	                pwmAwayPenaltyBottomSeconds.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmAwayPenaltyBottomSeconds.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case O :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyBottomSeconds.setLEDOff(i);
  	            } else {
  	                pwmAwayPenaltyBottomSeconds.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmAwayPenaltyBottomSeconds.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	        case U :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmPeriod.setLEDOff(i);
  	            } else {
  	                pwmPeriod.setLEDDimmed(i, brightness);
  	            }
				if(j == 7)
				{
					pwmPeriod.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	    }   //end case
  	    j ++;
  	} // end for
  	  	
  	update = 0;	//clear the flag, wait for next update
  } //end update block

  
}   //end loop

unsigned char sevenSegDecode(unsigned char number){
	//TODO
	//Check with matt about different display mappings (see display.xlsx)
	unsigned char decoded = 0;  //Consists of 1's and 0's that map to a segment of the display
	//Pattern for decodeType = 2 is XBACGFED
	//Pattern for decodeType = 1 is XDCBGAFE
	//Pattern for decodeType = 0 is XBAFGCED
	// X is don't care value for above
	switch(number)
	{
		case 0: //ABCDEF on
			//01110111
			if(decodeType == 2) decoded = 0x77;
			//01110111
			else if(decodeType == 1) decoded = 0x77;
			//01110111
			else if(decodeType == 0) decoded = 0x77;
			break;
		case 1: //BC on
			//01010000
			if(decodeType == 2) decoded = 0x50;
			//00110000
			else if(decodeType == 1) decoded = 0x30;
			//01000100
			else if(decodeType == 0) decoded = 0x44;
			break;
		case 2: //ABDEG on
			//01101011
			if(decodeType == 2) decoded = 0x6B;
			//01011101
			else if(decodeType == 1) decoded = 0x5D;
			//01101011
			else if(decodeType == 0) decoded = 0x6B;
			break;
		case 3: //ABCDG on
			//01111001
			if(decodeType == 2) decoded = 0x79;
			//01111100
			else if(decodeType == 1) decoded = 0x7C;
			//01101101
			else if(decodeType == 0) decoded = 0x6D;
			break;
		case 4: //BCFG on
			//01011100
			if(decodeType == 2) decoded = 0x5C;
			//00111010
			else if(decodeType == 1) decoded = 0x3A;
			//01011100
			else if(decodeType == 0) decoded = 0x5C;
			break;
		case 5: //ACDFG on
			//00111101
			if(decodeType == 2) decoded = 0x3D;
			//01101110
			else if(decodeType == 1) decoded = 0x6E;
			//00111101
			else if(decodeType == 0) decoded = 0x3D;
			break;
		case 6: //ACDEFG on
			//00111111
			if(decodeType == 2) decoded = 0x3F;
			//01101111
			else if(decodeType == 1) decoded = 0x6F;
			//00111111
			else if(decodeType == 0) decoded = 0x3F;
			break;
		case 7: //ABC on
			//01110000
			if(decodeType == 2) decoded = 0x70;
			//00110100
			else if(decodeType == 1) decoded = 0x34;
			//01100100
			else if(decodeType == 0) decoded = 0x64;
			break;
		case 8: //ABCDEFG on
			//01111111
			if(decodeType == 2) decoded = 0x7F;
			//01111111
			else if(decodeType == 1) decoded = 0x7F;
			//01111111
			else if(decodeType == 0) decoded = 0x7F;
			break;
		case 9: //ABCDFG on
			//01111101
			if(decodeType == 2) decoded = 0x7D;
			//01111110
			else if(decodeType == 1) decoded = 0x7E;
			//01111101
			else if(decodeType == 0) decoded = 0x7D;
			break;
	}
	return 0;
}


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
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Home Score 1s
		case G:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  decodeType = 1;
		  dotPin = -1;
		  break;
		//Away Score 10s
		case B:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Away Score 1s
		case A:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  decodeType = 1;
		  dotPin = -1;
		  break;
		//Time Minutes 10s
		case F:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Time Minutes 1s
		case E:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  decodeType = 1;
		  dotPin = 15;
		  break;
		//Time Seconds 10s
		case D:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Time Seconds 1s
		case C:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  decodeType = 1;
		  dotPin = 15;
		  break;
		//Home Penalty Top Minutes 1s
		case N:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 0;
		  dotPin = 8;
		  break;
		//Home Penalty Top Seconds 10s
		case M:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Home Penalty Top Seconds 1s
		case L:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-15
		  decodeType = 1;
		  dotPin = 15;
		  break;
		//Home Penalty Bottom Minutes 1s
		case T:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 0;
		  dotPin = 8;
		  break;
		//Home Penalty Bottom Seconds 10s
		case S:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Home Penalty Bottom Seconds 1s
		case R:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-15
		  decodeType = 1;
		  dotPin = 15;
		  break;
		//Away Penalty Top Minutes 1s
		case K:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 0;
		  dotPin = 8;
		  break;
		//Away Penalty Top Seconds 10s
		case J:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Away Penalty Top Seconds 1s
		case I:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-15
		  decodeType = 1;
		  dotPin = 15;
		  break;
		//Away Penalty Bottom Minutes 1s
		case Q:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 0;
		  dotPin = 8;
		  break;
		//Away Penalty Bottom Seconds 10s
		case P:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Away Penalty Bottom Seconds 1s
		case O:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  decodeType = 1;
		  dotPin = 15;
		  break;
		//Period
		case U:
		  chipAddr = currentAdr
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 0;
		  dotPin = -1;
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

