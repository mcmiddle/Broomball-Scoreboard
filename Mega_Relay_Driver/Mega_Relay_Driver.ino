//Code written by Jordan Grider, Michael Middleton, and Tino Moore for an Arduino Mega to control 7-segment output for a relay board
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
Adafruit_PWMServoDriver pwmHomeScore = Adafruit_PWMServoDriver(0x45);
Adafruit_PWMServoDriver pwmVisitorScore = Adafruit_PWMServoDriver(0x44); ;
Adafruit_PWMServoDriver pwmPeriod = Adafruit_PWMServoDriver(0x4F);
Adafruit_PWMServoDriver pwmTimeMinutes = Adafruit_PWMServoDriver(0x50);
Adafruit_PWMServoDriver pwmTimeSeconds = Adafruit_PWMServoDriver(0x51);
Adafruit_PWMServoDriver pwmHomePenaltyTopMinutes = Adafruit_PWMServoDriver(0x4B);
Adafruit_PWMServoDriver pwmHomePenaltyTopSeconds = Adafruit_PWMServoDriver(0x4C);
Adafruit_PWMServoDriver pwmHomePenaltyBottomMinutes = Adafruit_PWMServoDriver(0x4D);
Adafruit_PWMServoDriver pwmHomePenaltyBottomSeconds = Adafruit_PWMServoDriver(0x4E);
Adafruit_PWMServoDriver pwmAwayPenaltyTopMinutes = Adafruit_PWMServoDriver(0x47);
Adafruit_PWMServoDriver pwmAwayPenaltyTopSeconds = Adafruit_PWMServoDriver(0x48);
Adafruit_PWMServoDriver pwmAwayPenaltyBottomMinutes = Adafruit_PWMServoDriver(0x49);
Adafruit_PWMServoDriver pwmAwayPenaltyBottomSeconds = Adafruit_PWMServoDriver(0x4A);


void setup() {                

  Wire.begin();
  //Serial1.begin(9600);
  Serial.begin(9600);  // For debugging to terminal
  
  pwmHomeScore.begin();
  pwmVisitorScore.begin();
  pwmPeriod.begin();
  pwmTimeMinutes.begin();
  pwmTimeSeconds.begin();
  pwmHomePenaltyTopMinutes.begin();
  pwmHomePenaltyTopSeconds.begin();
  pwmHomePenaltyBottomMinutes.begin();
  pwmHomePenaltyBottomSeconds.begin();
  pwmAwayPenaltyTopMinutes.begin();
  pwmAwayPenaltyTopSeconds.begin();
  pwmAwayPenaltyBottomMinutes.begin();
  pwmAwayPenaltyBottomSeconds.begin();
  
  // update speed set to 100 Hz
  pwmHomeScore.setPWMFreq(100);
  pwmVisitorScore.setPWMFreq(100);
  pwmPeriod.setPWMFreq(100);
  pwmTimeMinutes.setPWMFreq(100);
  pwmTimeSeconds.setPWMFreq(100);
  pwmHomePenaltyTopMinutes.setPWMFreq(100);
  pwmHomePenaltyTopSeconds.setPWMFreq(100);
  pwmHomePenaltyBottomMinutes.setPWMFreq(100);
  pwmHomePenaltyBottomSeconds.setPWMFreq(100);
  pwmAwayPenaltyTopMinutes.setPWMFreq(100);
  pwmAwayPenaltyTopSeconds.setPWMFreq(100);
  pwmAwayPenaltyBottomMinutes.setPWMFreq(100);
  pwmAwayPenaltyBottomSeconds.setPWMFreq(100);
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
//Away Penalty Bottom Minutes 1s
update = 1;
//                //Home Score 10s
//	          chipAddr = 'H';
//		  chipStartPin = 1;	//1-7
//		  decodeType = 2;
//		  dotPin = -1;
//		  //Home Score 1s
//		  chipAddr = 'G';
//		  chipStartPin = 8;	//8-14
//		  decodeType = 1;
//		  dotPin = -1;
//		  //Away Score 10s
//		  chipAddr = 'B';
//		  chipStartPin = 1;	//1-7
//		  decodeType = 2;
//		  dotPin = -1;
//		  //Away Score 1s
//		  chipAddr = 'A';
//		  chipStartPin = 8;	//8-14
//		  decodeType = 1;
//		  dotPin = -1;
//		  //Time Minutes 10s
//		  chipAddr = 'F';
//		  chipStartPin = 1;	//1-7
//		  decodeType = 2;
//		  dotPin = -1;
//		  //Time Minutes 1s
//		  chipAddr = 'E';
//		  chipStartPin = 8;	//8-14
//		  decodeType = 1;
//		  dotPin = 15;
//		  //Time Seconds 1s
//		  chipAddr = 'D';
//		  chipStartPin = 1;	//1-7
//		  decodeType = 3;
//		  dotPin = -1;
//		  //Time Seconds 10s
//		  chipAddr = 'C';
//		  chipStartPin = 8;	//8-14
//		  decodeType = 4;
//		  dotPin = 15;
//		  //Home Penalty Top Minutes 1s
//		  chipAddr = 'N';
//		  chipStartPin = 1;	//1-7
//		  decodeType = 0;
//		  dotPin = 8;
//		  //Home Penalty Top Seconds 10s
//		  chipAddr = 'M';
//		  chipStartPin = 1;	//1-7
//		  decodeType = 2;
//		  dotPin = -1;
//		  //Home Penalty Top Seconds 1s
//		  chipAddr = 'L';
//		  chipStartPin = 8;	//8-15
//		  decodeType = 5;
//		  dotPin = -1;
//		  //Home Penalty Bottom Minutes 1s
//		  chipAddr = 'T';
//		  chipStartPin = 1;	//1-7
//		  decodeType = 0;
//		  dotPin = 8;
//		  //Home Penalty Bottom Seconds 10s
//		  chipAddr = 'S';
//		  chipStartPin = 1;	//1-7
//		  decodeType = 2;
//		  dotPin = -1;
//		  //Home Penalty Bottom Seconds 1s
//		  chipAddr = 'R';
//		  chipStartPin = 8;	//8-15
//		  decodeType = 5;
//		  dotPin = -1;
//		  //Away Penalty Top Minutes 1s
//		  chipAddr = 'K';
//		  chipStartPin = 1;	//1-7
//		  decodeType = 0;
//		  dotPin = 8;
//		  //Away Penalty Top Seconds 10s
//		  chipAddr = 'J';
//		  chipStartPin = 1;	//1-7
//		  decodeType = 2;
//		  dotPin = -1;
//		  //Away Penalty Top Seconds 1s
//		  chipAddr = 'I';
//		  chipStartPin = 8;	//8-15
//		  decodeType = 5;
//		  dotPin = -1;
//		  //Away Penalty Bottom Minutes 1s
//		  chipAddr = 'Q';
//		  chipStartPin = 1;	//1-7
//		  decodeType = 0;
//		  dotPin = 8;
//		  //Away Penalty Bottom Seconds 10s
//		  chipAddr = 'P';
//		  chipStartPin = 1;	//1-7
//		  decodeType = 2;
//		  dotPin = -1;
//		  //Away Penalty Bottom Seconds 1s
//		  chipAddr = 'O';
//		  chipStartPin = 8;	//8-14
//		  decodeType = 5;
//		  dotPin = -1;
//		  //Period
//		  chipAddr = 'U';
//		  chipStartPin = 1;	//1-7
//		  decodeType = 6;
//		  dotPin = -1;
  
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
      	            pwmHomeScore.setPin(i, 0, 0);
  	            } else {
  	                pwmHomeScore.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmHomeScore.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'G' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomeScore.setPin(i, 0, 0);
  	            } else {
  	                pwmHomeScore.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmHomeScore.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'B' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmVisitorScore.setPin(i, 0, 0);
  	            } else {
  	                pwmVisitorScore.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmVisitorScore.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'A' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmVisitorScore.setPin(i, 0, 0);
  	            } else {
  	                pwmVisitorScore.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmVisitorScore.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'F' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmTimeMinutes.setPin(i, 0, 0);
  	            } else {
  	                pwmTimeMinutes.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmTimeMinutes.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'E' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmTimeMinutes.setPin(i, 0, 0);
  	            } else {
  	                pwmTimeMinutes.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmTimeMinutes.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'D' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmTimeSeconds.setPin(i, 0, 0);
  	            } else {
  	                pwmTimeSeconds.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmTimeSeconds.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'C' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmTimeSeconds.setPin(i, 0, 0);
  	            } else {
  	                pwmTimeSeconds.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmTimeSeconds.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'N' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyTopMinutes.setPin(i, 0, 0);
                    
  	            } else {
  	                pwmHomePenaltyTopMinutes.setPin(i, brightness, 0);
                        
  	            }
				if(j == 6)
				{
					pwmHomePenaltyTopMinutes.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'M' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyTopSeconds.setPin(i, 0, 0);
  	            } else {
  	                pwmHomePenaltyTopSeconds.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmHomePenaltyTopSeconds.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'L' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyTopSeconds.setPin(i, 0, 0);
  	            } else {
  	                pwmHomePenaltyTopSeconds.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmHomePenaltyTopSeconds.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'T' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyBottomMinutes.setPin(i, 0, 0);
  	            } else {
  	                pwmHomePenaltyBottomMinutes.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmHomePenaltyBottomMinutes.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'S' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyBottomSeconds.setPin(i, 0, 0);
  	            } else {
  	                pwmHomePenaltyBottomSeconds.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmHomePenaltyBottomSeconds.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'R' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmHomePenaltyBottomSeconds.setPin(i, 0, 0);
  	            } else {
  	                pwmHomePenaltyBottomSeconds.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmHomePenaltyBottomSeconds.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'K' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyTopMinutes.setPin(i, 0, 0);
  	            } else {
  	                pwmAwayPenaltyTopMinutes.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmAwayPenaltyTopMinutes.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'J' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyTopSeconds.setPin(i, 0, 0);
  	            } else {
  	                pwmAwayPenaltyTopSeconds.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmAwayPenaltyTopSeconds.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'I' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyTopSeconds.setPin(i, 0, 0);
  	            } else {
  	                pwmAwayPenaltyTopSeconds.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmAwayPenaltyTopSeconds.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'Q' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyBottomMinutes.setPin(i, 0, 0);
  	            } else {
  	                pwmAwayPenaltyBottomMinutes.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmAwayPenaltyBottomMinutes.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'P' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyBottomSeconds.setPin(i, 0, 0);
  	            } else {
  	                pwmAwayPenaltyBottomSeconds.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmAwayPenaltyBottomSeconds.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'O' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
      	            pwmAwayPenaltyBottomSeconds.setPin(i, 0, 0);
  	            } else {
  	                pwmAwayPenaltyBottomSeconds.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmAwayPenaltyBottomSeconds.setPin(dotPin, brightness, 0);
				}
  	            break;
  	        case 'U' :
  	            if(((decodedValue >> j) & 0x01) == 0){   
                        pwmPeriod.setPin(i, 0, 0);
  	            } 
                    else {
                        pwmPeriod.setPin(i, brightness, 0);
  	            }
				if(j == 6)
				{
					pwmPeriod.setPin(dotPin, brightness, 0);
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
	//Time, Penalties, and Score 10s digit: decodeType = 2 for normal and 3 for inverted. Pattern is XBACGFED
	//Time and Score 1s digit: decodeType = 1 for normal and 4 for inverted. Pattern is XDCBGAFE
        //Penalty 1s digit: decodeType = 5. Pattern is XCBDGAFE
        //Period: decodeType = 6. Pattern is XBAFGCED
	//Penalty Minutes: decodeType = 0. Pattern is XABFGCED
	// X is don't care value for above
	switch(number)
	{
		case 0: //ABCDEF on normal and inverted
			//01110111
			if(decodeType == 2 || decodeType == 3) decoded = 0x77;
			//01110111
			else if(decodeType == 1 || decodeType == 4) decoded = 0x77;
                        //01110111
			else if(decodeType == 5) decoded = 0x77;
			//01110111
			else if(decodeType == 6) decoded = 0x77;
			//01110111
			else if(decodeType == 0) decoded = 0x77;
			break;
		case 1: //BC on normal, EF on inverted
			//01010000
			if(decodeType == 2) decoded = 0x50;
			//00000110
			else if(decodeType == 3) decoded = 0x06;
			//00110000
			else if(decodeType == 1) decoded = 0x30;
			//00000011
			else if(decodeType == 4) decoded = 0x03;
                        //01100000
			else if(decodeType == 5) decoded = 0x60;
			//01000100
			else if(decodeType == 6) decoded = 0x44;
			//00100100
			else if(decodeType == 0) decoded = 0x24;
			break;
		case 2: //ABDEG on normal and inverted
			//01101011
			if(decodeType == 2 || decodeType == 3) decoded = 0x6B;
			//01011101
			else if(decodeType == 1 || decodeType == 4) decoded = 0x5D;
                        //00111101
			else if(decodeType == 5) decoded = 0x3D;
			//01101011
			else if(decodeType == 6) decoded = 0x6B;
			//01101011
			else if(decodeType == 0) decoded = 0x6B;
			break;
		case 3: //ABCDG on normal, ADEFG on inverted
			//01111001
			if(decodeType == 2) decoded = 0x79;
			//00101111
			else if(decodeType == 3) decoded = 0x2F;
			//01111100
			else if(decodeType == 1) decoded = 0x7C;
			//01001111
			else if(decodeType == 4) decoded = 0x4F;
                        //01111100
			else if(decodeType == 5) decoded = 0x7C;
			//01101101
			else if(decodeType == 6) decoded = 0x6D;
			//01101101
			else if(decodeType == 0) decoded = 0x6D;
			break;
		case 4: //BCFG on normal, CEFG on inverted
			//01011100
			if(decodeType == 2) decoded = 0x5C;
			//00011110
			else if(decodeType == 3) decoded = 0x1E;
			//00111010
			else if(decodeType == 1) decoded = 0x3A;
			//00101011
			else if(decodeType == 4) decoded = 0x2B;
                        //01101010
			else if(decodeType == 5) decoded = 0x6A;
			//01011100
			else if(decodeType == 6) decoded = 0x5C;
			//00111100
			else if(decodeType == 0) decoded = 0x3C;
			break;
		case 5: //ACDFG on normal and inverted
			//00111101
			if(decodeType == 2 || decodeType == 3) decoded = 0x3D;
			//01101110
			else if(decodeType == 1 || decodeType == 4) decoded = 0x6E;
                        //01011110
			else if(decodeType == 5) decoded = 0x5E;
			//00111101
			else if(decodeType == 6) decoded = 0x3D;
			//01011101
			else if(decodeType == 0) decoded = 0x5D;
			break;
		case 6: //ACDEFG on normal, ABCDFG on inverted
			//00111111
			if(decodeType == 2) decoded = 0x3F;
			//01111101
			else if(decodeType == 3) decoded = 0x7D;
			//01101111
			else if(decodeType == 1) decoded = 0x6F;
			//01111110
			else if(decodeType == 4) decoded = 0x7E;
                        //01011111
			else if(decodeType == 5) decoded = 0x5F;
			//00111111
			else if(decodeType == 6) decoded = 0x3F;
			//01011111
			else if(decodeType == 0) decoded = 0x5F;
			break;
		case 7: //ABC on normal, DEF on inverted
			//01110000
			if(decodeType == 2) decoded = 0x70;
			//00000111
			else if(decodeType == 3) decoded = 0x07;
			//00110100
			else if(decodeType == 1) decoded = 0x34;
			//01000011
			else if(decodeType == 4) decoded = 0x43;
                        //01100100
			else if(decodeType == 5) decoded = 0x64;
			//01100100
			else if(decodeType == 6) decoded = 0x64;
			//01100100
			else if(decodeType == 0) decoded = 0x64;
			break;
		case 8: //ABCDEFG on normal and inverted
			//01111111
			if(decodeType == 2 || decodeType == 3) decoded = 0x7F;
			//01111111
			else if(decodeType == 1 || decodeType == 4) decoded = 0x7F;
                        //01111111
			else if(decodeType == 5) decoded = 0x7F;
			//01111111
			else if(decodeType == 6) decoded = 0x7F;
			//01111111
			else if(decodeType == 0) decoded = 0x7F;
			break;
		case 9: //ABCDFG on normal, ACDEFG on inverted
			//01111101
			if(decodeType == 2) decoded = 0x7D;
			//00111111
			else if(decodeType == 3) decoded = 0x3F;
			//01111110
			else if(decodeType == 1) decoded = 0x7E;
			//01101111
			else if(decodeType == 4) decoded = 0x6F;
                        //01111110
			else if(decodeType == 5) decoded = 0x7E;
			//01111101
			else if(decodeType == 6) decoded = 0x7D;
			//01111101
			else if(decodeType == 0) decoded = 0x7D;
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
		//Time Seconds 1s
		case 'D':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 3;
		  dotPin = -1;
		  break;
		//Time Seconds 10s
		case 'C':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 8;	//8-14
		  decodeType = 4;
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
		  decodeType = 5;
		  dotPin = -1;
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
		  decodeType = 5;
		  dotPin = -1;
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
		  decodeType = 5;
		  dotPin = -1;
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
		  decodeType = 5;
		  dotPin = -1;
		  break;
		//Period
		case 'U':
		  chipAddr = currentAddr;
		  writeValue = number;
		  chipStartPin = 1;	//1-7
		  decodeType = 6;
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

