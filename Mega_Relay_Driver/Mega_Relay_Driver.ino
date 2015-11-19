//Code written by Jordan Grider, Michael Middleton, and Tino Moore for an Arduino Mega to control 7-segment output for a relay board
#include "PCA9685.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

unsigned char update = 0; 	//flag set when new data received from ref-controller
unsigned char chipAddr = 0; //Address of chip to update
unsigned char writeValue = 0; //Value to be displayed
char chipStartPin = 0; //Pin corresponding to starting location of the display being updated at the given chip address
byte brightness= 100;    //TODO set initial value
char dotPin = 0;  //Pin corresponding to the dot, -1 if no dot
char decodeType = -1; //There are two different pin mappings for display depending on single digit or two digits
					  //value is used for decoding each type. 0 is one digit by itself, 1 is first digit of two, 2 is second digit of two, 3 is 10s inverted, 4 is 1s inverted

int led = 22;
int inint = 0;
char incoming = 'a';
boolean serialRX = false;

char currentAddr = 'Z';
PCA9685 pwmHomeScore;
PCA9685 pwmVisitorScore;
//PCA9685 pwmPeriod;
PCA9685 pwmTimeMinutes;
PCA9685 pwmTimeSeconds;
PCA9685 pwmHomePenaltyTopMinutes;
PCA9685 pwmHomePenaltyTopSeconds;
PCA9685 pwmHomePenaltyBottomMinutes;
PCA9685 pwmHomePenaltyBottomSeconds;
PCA9685 pwmAwayPenaltyTopMinutes;
PCA9685 pwmAwayPenaltyTopSeconds;
PCA9685 pwmAwayPenaltyBottomMinutes;
PCA9685 pwmAwayPenaltyBottomSeconds;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x4F);

void setup() {                

  Wire.begin();
  //Serial1.begin(9600);
  Serial.begin(9600);  // For debugging to terminal
  
  pwmHomeScore.begin(10001010);
  pwmVisitorScore.begin(10001000);
  //pwmPeriod.begin(1001111);
  pwmTimeMinutes.begin(10100000);
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
  //pwmPeriod.init();
  pwmTimeMinutes.init();
  pwmTimeSeconds.init();
  pwmHomePenaltyTopMinutes.init();
  pwmHomePenaltyTopSeconds.init();
  pwmHomePenaltyBottomMinutes.init();
  pwmHomePenaltyBottomSeconds.init();
  pwmAwayPenaltyTopMinutes.init();
  pwmAwayPenaltyTopSeconds.init();
  pwmAwayPenaltyBottomMinutes.init();
  pwmAwayPenaltyBottomSeconds.init();
  
  pwm.begin();
  
  pwm.setPWMFreq(100);  // update speed set to 100 Hz
}


void loop() {
    unsigned char decodedValue = 0;
    int i, j;
/*	
  if (serialRX && Debugger){
	Serial.println(chipAddr);
    Serial.println(writeValue);
	Serial.println(chipPins);
    serialRX = false;
  }  
  */
  //TODO handle brightness logic
  chipAddr = 'U';
  chipStartPin = 1;	//1-7
  decodeType = 0;
  update = 1;
  dotPin = -1;
  if (writeValue == 9) writeValue = 0;
  else writeValue++;
  
  //Display update logic
  if (update == 1){
  	//handle horn operation
  	    //TODO
  	
  	//Decode the display value
  	decodedValue = sevenSegDecode(writeValue);
  	Serial.println(decodedValue);
  	//Write the value to the display
        j = 0;
  	for (i = chipStartPin; i < chipStartPin + 7; i ++){  
  	    //case statement to determine which pwm object to apply changes to based on chipAddr
  	    switch(chipAddr){
  	        case 'H' :
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
  	        case 'G' :
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
  	        case 'B' :
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
  	        case 'A' :
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
  	        case 'F' :
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
  	        case 'E' :
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
  	        case 'D' :
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
  	        case 'C' :
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
  	        case 'N' :
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
  	        case 'M' :
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
  	        case 'L' :
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
  	        case 'T' :
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
  	        case 'S' :
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
  	        case 'R' :
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
  	        case 'K' :
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
  	        case 'J' :
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
  	        case 'I' :
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
  	        case 'Q' :
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
  	        case 'P' :
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
  	        case 'O' :
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
  	        case 'U' :
                    Serial.println(i);
  	            if(((decodedValue >> j) & 0x01) == 0){   
                      Serial.println("Writing Off");
      	             // pwmPeriod.setLEDOff(i);
                        pwm.setPin(i, 0, 0);
  	            } 
                    else {
                        Serial.println("Writing On");
  	                //pwmPeriod.setLEDDimmed(i, brightness);
                        pwm.setPin(i, brightness, 0);
  	            }
				if(j == 7)
				{
					//pwmPeriod.setLEDDimmed(dotPin, brightness);
				}
  	            break;
  	    }   //end case
  	    j ++;
  	} // end for
  	  	
  	//update = 0;	//clear the flag, wait for next update
  } //end update block
  delay(1000);
  
}   //end loop

unsigned char sevenSegDecode(unsigned char number){
	//TODO
	//Check with matt about different display mappings (see display.xlsx)
	unsigned char decoded = 0;  //Consists of 1's and 0's that map to a segment of the display
	//Pattern for decodeType = 2 and 3 is XBACGFED
	//Pattern for decodeType = 1 and 4 is XDCBGAFE
	//Pattern for decodeType = 0 is XBAFGCED
	// X is don't care value for above
	switch(number)
	{
		case 0: //ABCDEF on normal and inverted
			//01110111
			if(decodeType == 2 || decodeType == 3) decoded = 0x77;
			//01110111
			else if(decodeType == 1 || decodeType == 4) decoded = 0x77;
			//01110111
			else if(decodeType == 0) decoded = 0x77;
			break;
		case 1: //BC on normal, EF on inverted
			//01010000
			if(decodeType == 2) decoded = 0x50;
			//00110000
			else if(decodeType == 1) decoded = 0x30;
			//01000100
			else if(decodeType == 0) decoded = 0x44;
			//00000110
			else if(decodeType == 3) decoded = 0x06;
			//00000011
			else if(decodeType == 4) decoded = 0x03;
			break;
		case 2: //ABDEG on normal and inverted
			//01101011
			if(decodeType == 2 || decodeType == 3) decoded = 0x6B;
			//01011101
			else if(decodeType == 1 || decodeType == 4) decoded = 0x5D;
			//01101011
			else if(decodeType == 0) decoded = 0x6B;
			break;
		case 3: //ABCDG on normal, ADEFG on inverted
			//01111001
			if(decodeType == 2) decoded = 0x79;
			//01111100
			else if(decodeType == 1) decoded = 0x7C;
			//01101101
			else if(decodeType == 0) decoded = 0x6D;
			//00101111
			else if(decodeType == 3) decoded = 0x2F;
			//01001111
			else if(decodeType == 4) decoded = 0x4F;
			break;
		case 4: //BCFG on normal, CEFG on inverted
			//01011100
			if(decodeType == 2) decoded = 0x5C;
			//00111010
			else if(decodeType == 1) decoded = 0x3A;
			//01011100
			else if(decodeType == 0) decoded = 0x5C;
			//00011110
			else if(decodeType == 3) decoded = 0x1E;
			//00101011
			else if(decodeType == 4) decoded = 0x2B;
			break;
		case 5: //ACDFG on normal and inverted
			//00111101
			if(decodeType == 2 || decodeType == 3) decoded = 0x3D;
			//01101110
			else if(decodeType == 1 || decodeType == 4) decoded = 0x6E;
			//00111101
			else if(decodeType == 0) decoded = 0x3D;
			break;
		case 6: //ACDEFG on normal, ABCDFG on inverted
			//00111111
			if(decodeType == 2) decoded = 0x3F;
			//01101111
			else if(decodeType == 1) decoded = 0x6F;
			//00111111
			else if(decodeType == 0) decoded = 0x3F;
			//01111101
			else if(decodeType == 3) decoded = 0x7D;
			//01111110
			else if(decodeType == 4) decoded = 0x7E;
			break;
		case 7: //ABC on normal, DEF on inverted
			//01110000
			if(decodeType == 2) decoded = 0x70;
			//00110100
			else if(decodeType == 1) decoded = 0x34;
			//01100100
			else if(decodeType == 0) decoded = 0x64;
			//00000111
			else if(decodeType == 3) decoded = 0x07;
			//01000011
			else if(decodeType == 4) decoded = 0x43;
			break;
		case 8: //ABCDEFG on normal and inverted
			//01111111
			if(decodeType == 2 || decodeType == 3) decoded = 0x7F;
			//01111111
			else if(decodeType == 1 || decodeType == 4) decoded = 0x7F;
			//01111111
			else if(decodeType == 0) decoded = 0x7F;
			break;
		case 9: //ABCDFG on normal, ACDEFG on inverted
			//01111101
			if(decodeType == 2) decoded = 0x7D;
			//01111110
			else if(decodeType == 1) decoded = 0x7E;
			//01111101
			else if(decodeType == 0) decoded = 0x7D;
			//00111111
			else if(decodeType == 3) decoded = 0x3F;
			//01101111
			else if(decodeType == 4) decoded = 0x6F;
			break;
	}
	return decoded;
}


void serialEvent1() {
  //TODO Change back to Serial1
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    //Serial1.print(inChar);

    int number = inChar - '0';
    if (inChar == '/'){
      number = -1;
    }

	// If a number, display. Else set it as address.
    if (number < 10 && number >= -1){
	  switch(currentAddr){
		//Home Score 10s
	    case 'H':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Home Score 1s
		case 'G':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  decodeType = 1;
		  dotPin = -1;
		  break;
		//Away Score 10s
		case 'B':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Away Score 1s
		case 'A':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  decodeType = 1;
		  dotPin = -1;
		  break;
		//Time Minutes 10s
		case 'F':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Time Minutes 1s
		case 'E':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  decodeType = 1;
		  dotPin = 15;
		  break;
		//TODO Inversion
		//Time Seconds 1s
		case 'D':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 4;
		  dotPin = -1;
		  break;
		//Time Seconds 10s
		case 'C':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  decodeType = 3;
		  dotPin = 15;
		  break;
		//Home Penalty Top Minutes 1s
		case 'N':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 0;
		  dotPin = 8;
		  break;
		//Home Penalty Top Seconds 10s
		case 'M':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Home Penalty Top Seconds 1s
		case 'L':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 8;	//8-15
		  decodeType = 1;
		  dotPin = 15;
		  break;
		//Home Penalty Bottom Minutes 1s
		case 'T':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 0;
		  dotPin = 8;
		  break;
		//Home Penalty Bottom Seconds 10s
		case 'S':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Home Penalty Bottom Seconds 1s
		case 'R':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 8;	//8-15
		  decodeType = 1;
		  dotPin = 15;
		  break;
		//Away Penalty Top Minutes 1s
		case 'K':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 0;
		  dotPin = 8;
		  break;
		//Away Penalty Top Seconds 10s
		case 'J':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Away Penalty Top Seconds 1s
		case 'I':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 8;	//8-15
		  decodeType = 1;
		  dotPin = 15;
		  break;
		//Away Penalty Bottom Minutes 1s
		case 'Q':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 0;
		  dotPin = 8;
		  break;
		//Away Penalty Bottom Seconds 10s
		case 'P':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 2;
		  dotPin = -1;
		  break;
		//Away Penalty Bottom Seconds 1s
		case 'O':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  decodeType = 1;
		  dotPin = 15;
		  break;
		//Period
		case 'U':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 0;
		  dotPin = -1;
		  break;
		//Horn
		case 'V':
		  chipAddr = currentAddr;
		  writeValue = number;
		  //chipPins;
		  
		  break;
	  }
	  
      currentAddr = 'Z';
      update = 1;
    }

    currentAddr = inChar;

  }
  serialRX = true;
}

