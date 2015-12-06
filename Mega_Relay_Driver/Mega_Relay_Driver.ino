//Code written by Jordan Grider, Michael Middleton, and Tino Moore for an Arduino Mega to control 7-segment output for a relay board
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

unsigned char update = 0; 	//flag set when new data received from ref-controller
unsigned char chipAddr = 0; //Address of chip to update
unsigned char writeValue = 0; //Value to be displayed
char chipStartPin = 0; //Pin corresponding to starting location of the display being updated at the given chip address
int brightness = 0; 
char newBrightness = 0;
char dotPin = 0;  //Pin corresponding to the dot, -1 if no dot
char decodeType = -1; //There are two different pin mappings for display depending on single digit or two digits
					  //value is used for decoding each type. 0 is one digit by itself, 1 is first digit of two, 2 is second digit of two, 3 is 10s inverted, 4 is 1s inverted
boolean serialRX = false;
boolean mainController = false; //For use when detecting brightness
int currValues [22]; // Array of most recent value for each display for use in brightness logic

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
  
  int sensorValue = analogRead(A0);
  brightness = sensorValue * 4;
  
  for(int i = 0; i < currValues.length(); i++)
  {
	currValues[i] = 0;
  }
  
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
  if(mainController)
  {
	int sensorValue = analogRead(A0);
	newBrightness = sensorValue / 4; //Reduce to size of char for serial transport
	Serial.print('Y');
	Serial.write(newBrightness); //Use write instead of print to send raw data
  }
  else
  {
	int tempBrightness = newBrightness * 4;
	if(((tempBrightness - newBrightness) > 500) || ((newBrightness - tempBrightness) > 500))
	{
		updateBrightness(tempBrightness);
	}
  }
  
  
  //Display update logic
  if (update == 1){
	if(chipAddr == 'V')
	{
		pinMode(2, OUTPUT); //Set digital pin 2 as output for horn
		digitalWrite(2, HIGH); 
	}
  	
  	//Decode the display value
  	decodedValue = sevenSegDecode(writeValue);
  	Serial.println(decodedValue);
  	//Write the value to the display
    j = 0;
  	for (i = chipStartPin; i < chipStartPin + 7; i ++){  
  	    //case statement to determine which pwm object to apply changes to based on chipAddr
  	    switch(chipAddr){
  	        case 'H' :
				currValues[0] = decodedValue;
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
				currValues[1] = decodedValue;
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
				currValues[2] = decodedValue;
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
				currValues[3] = decodedValue;
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
				currValues[4] = decodedValue;
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
				currValues[5] = decodedValue;
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
				currValues[6] = decodedValue;
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
				currValues[7] = decodedValue;
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
				currValues[8] = decodedValue;
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
				currValues[9] = decodedValue;
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
				currValues[10] = decodedValue;
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
				currValues[11] = decodedValue;
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
				currValues[12] = decodedValue;
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
				currValues[13] = decodedValue;
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
				currValues[14] = decodedValue;
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
				currValues[15] = decodedValue;
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
				currValues[16] = decodedValue;
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
				currValues[17] = decodedValue;
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
				currValues[18] = decodedValue;
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
				currValues[19] = decodedValue;
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
				currValues[20] = decodedValue;
				if(j == 0)
				{
					updateBrightness(newBrightness); //Update LED brightness in between periods
				}
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
  	  	
  	update = 0;	//clear the flag, wait for next update
  } //end update block
  delay(1000);
  
}   //end loop

unsigned char sevenSegDecode(unsigned char number){
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
  while (Serial.available()) {
	//Recieve data in on RX and send it out on TX to next Arduino
    char inChar = (char)Serial.read();
    Serial.print(inChar);

    int number = inChar - '0';
    if (inChar == '/'){
      number = -1;
    }
	//Horn
	if(currentAddr == 'V')
	{
		chipAddr = currentAddr;
		writeValue = number;
	}
	// Brightness Update
	else if(currentAddr == 'Y')
	{
		newBrightness = inChar;
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
	  }
      currentAddr = 'Z';
      update = 1;
    }

    currentAddr = inChar;

  }
  serialRX = true;
}

void updateBrightness(int tempBrightness)
{
  	for (int i = 1; i < 8; i ++){  
		if(((currValues[0] >> i) & 0x01) == 0){   
			pwmHomeScore.setPin(i, 0, 0);
		} else {
			pwmHomeScore.setPin(i, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmHomeScore.setPin(-1, tempBrightness, 0);
		}
		
		if(((currValues[1] >> i) & 0x01) == 0){   
			pwmHomeScore.setPin(i+7, 0, 0);
		} else {
			pwmHomeScore.setPin(i+7, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmHomeScore.setPin(-1, tempBrightness, 0);
		}
		
		if(((currValues[2] >> i) & 0x01) == 0){   
			pwmVisitorScore.setPin(i, 0, 0);
		} else {
			pwmVisitorScore.setPin(i, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmVisitorScore.setPin(-1, tempBrightness, 0);
		}
		
		if(((currValues[3] >> i) & 0x01) == 0){   
			pwmVisitorScore.setPin(i+7, 0, 0);
		} else {
			pwmVisitorScore.setPin(i+7, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmVisitorScore.setPin(-1, tempBrightness, 0);
		}
		
		if(((currValues[4] >> i) & 0x01) == 0){   
			pwmTimeMinutes.setPin(i, 0, 0);
		} else {
			pwmTimeMinutes.setPin(i, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmTimeMinutes.setPin(-1, tempBrightness, 0);
		}
		
		if(((currValues[5] >> i) & 0x01) == 0){   
			pwmTimeMinutes.setPin(i+7, 0, 0);
		} else {
			pwmTimeMinutes.setPin(i+7, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmTimeMinutes.setPin(15, tempBrightness, 0);
		}
		
		if(((currValues[6] >> i) & 0x01) == 0){   
			pwmTimeSeconds.setPin(i, 0, 0);
		} else {
			pwmTimeSeconds.setPin(i, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmTimeSeconds.setPin(-1, tempBrightness, 0);
		}
		
		if(((currValues[7] >> i) & 0x01) == 0){   
			pwmTimeSeconds.setPin(i+7, 0, 0);
		} else {
			pwmTimeSeconds.setPin(i+7, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmTimeSeconds.setPin(15, tempBrightness, 0);
		}
		
		if(((currValues[8] >> i) & 0x01) == 0){   
			pwmHomePenaltyTopMinutes.setPin(i, 0, 0);
			
		} else {
			pwmHomePenaltyTopMinutes.setPin(i, tempBrightness, 0);
				
		}
		if(i == 6)
		{
			pwmHomePenaltyTopMinutes.setPin(8, tempBrightness, 0);
		}
		
		if(((currValues[9] >> i) & 0x01) == 0){   
			pwmHomePenaltyTopSeconds.setPin(i, 0, 0);
		} else {
			pwmHomePenaltyTopSeconds.setPin(i, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmHomePenaltyTopSeconds.setPin(-1, tempBrightness, 0);
		}
		
		if(((currValues[10] >> i) & 0x01) == 0){   
			pwmHomePenaltyTopSeconds.setPin(i+7, 0, 0);
		} else {
			pwmHomePenaltyTopSeconds.setPin(i+7, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmHomePenaltyTopSeconds.setPin(-1, tempBrightness, 0);
		}
		
		if(((currValues[11] >> i) & 0x01) == 0){   
			pwmHomePenaltyBottomMinutes.setPin(i, 0, 0);
		} else {
			pwmHomePenaltyBottomMinutes.setPin(i, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmHomePenaltyBottomMinutes.setPin(8, tempBrightness, 0);
		}
		
		if(((currValues[12] >> i) & 0x01) == 0){   
			pwmHomePenaltyBottomSeconds.setPin(i, 0, 0);
		} else {
			pwmHomePenaltyBottomSeconds.setPin(i, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmHomePenaltyBottomSeconds.setPin(-1, tempBrightness, 0);
		}
		
		if(((currValues[13] >> i) & 0x01) == 0){   
			pwmHomePenaltyBottomSeconds.setPin(i+7, 0, 0);
		} else {
			pwmHomePenaltyBottomSeconds.setPin(i+7, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmHomePenaltyBottomSeconds.setPin(-1, tempBrightness, 0);
		}
		
		if(((currValues[14] >> i) & 0x01) == 0){   
			pwmAwayPenaltyTopMinutes.setPin(i, 0, 0);
		} else {
			pwmAwayPenaltyTopMinutes.setPin(i, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmAwayPenaltyTopMinutes.setPin(8, tempBrightness, 0);
		}
		
		if(((currValues[15] >> i) & 0x01) == 0){   
			pwmAwayPenaltyTopSeconds.setPin(i, 0, 0);
		} else {
			pwmAwayPenaltyTopSeconds.setPin(i, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmAwayPenaltyTopSeconds.setPin(-1, tempBrightness, 0);
		}
		
		if(((currValues[16] >> i) & 0x01) == 0){   
			pwmAwayPenaltyTopSeconds.setPin(i+7, 0, 0);
		} else {
			pwmAwayPenaltyTopSeconds.setPin(i+7, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmAwayPenaltyTopSeconds.setPin(-1, tempBrightness, 0);
		}
		
		if(((currValues[17] >> i) & 0x01) == 0){   
			pwmAwayPenaltyBottomMinutes.setPin(i, 0, 0);
		} else {
			pwmAwayPenaltyBottomMinutes.setPin(i, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmAwayPenaltyBottomMinutes.setPin(8, tempBrightness, 0);
		}

		if(((currValues[18] >> i) & 0x01) == 0){   
			pwmAwayPenaltyBottomSeconds.setPin(i, 0, 0);
		} else {
			pwmAwayPenaltyBottomSeconds.setPin(i, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmAwayPenaltyBottomSeconds.setPin(-1, tempBrightness, 0);
		}

		if(((currValues[19] >> i) & 0x01) == 0){   
			pwmAwayPenaltyBottomSeconds.setPin(i+7, 0, 0);
		} else {
			pwmAwayPenaltyBottomSeconds.setPin(i+7, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmAwayPenaltyBottomSeconds.setPin(-1, tempBrightness, 0);
		}

		if(((currValues[20] >> i) & 0x01) == 0){   
				pwmPeriod.setPin(i, 0, 0);
		} 
			else {
				pwmPeriod.setPin(i, tempBrightness, 0);
		}
		if(i == 6)
		{
			pwmPeriod.setPin(-1, tempBrightness, 0);
		}
  	} // end for
}

