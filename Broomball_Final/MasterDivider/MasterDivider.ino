#include <Wire.h>


void setup() {
  // put your setup code here, to run once:
  delay(3000);
  
  Wire.begin();
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  
  pinMode(A0,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void serialEvent(){
  char addr;
  char data;
  unsigned char sensorValue;  
  char newBrightness;
  
   while (Serial.available() >= 2){
     
     //Serial.println(Serial.available());
      addr = (char)Serial.read();
      data = (char)Serial.read();
      
      if (addr == 'Y'){
        Serial1.print('Y');
	Serial1.print(data); 
        Serial2.print('Y');
	Serial2.print(data); 
        Serial3.print('Y');
	Serial3.print(data); 
      } 
	  //Send reset signal to all chips
	  else if(addr == 'Z'){
			Serial1.print(addr);
			Serial1.print(data);
			Serial2.print(addr);
			Serial2.print(data);
			Serial3.print(addr);
			Serial3.print(data);
			delay(1000); // Wait until it comes back up
	  }
		else if (addr == 'V'){
        if (data == '8'){
          digitalWrite(24,HIGH);
        } else {
          digitalWrite(24,LOW);
        }
      } else if (addr == 'C' ||addr == 'D' ||addr == 'E' ||addr == 'F' || addr == 'U'){
        //time center
        Serial3.print(addr);
        Serial3.print(data);
      } else if (addr == 'A' || addr == 'B' || addr == 'I' || addr == 'J' || addr == 'K' || addr == 'O' || addr == 'P' || addr == 'Q'){
        //away side
        Serial1.print(addr);
        Serial1.print(data);
      } else if (addr == 'G' || addr == 'H' ||addr == 'L' || addr == 'M' ||addr == 'N' ||addr == 'R' ||addr == 'S' ||addr == 'T'){
        //home side
        Serial2.print(addr);
        Serial2.print(data);
      }
   }
}
