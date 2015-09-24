//Code written by Jordan Grider to for an Arduino Mega to control 7-segment output for a relay board

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

int led = 22;
int inint = 0;
char incoming = 'a';
boolean serialRX = false;

boolean seg1change = false;
boolean seg2change = false;
boolean seg3change = false;
boolean seg4change = false;

char currentaddr = 'Z';

int seg1val = -1;
int seg2val = -1;
int seg3val = -1;
int seg4val = -1;


void setup() {                
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

  Serial1.begin(9600);
  Serial.begin(9600);  

}


void loop() {

  if (serialRX && Debugger){
    Serial.println(seg1val);
    Serial.println(seg2val);
    Serial.println(seg3val);
    Serial.println(seg4val);
    serialRX = false;
  }  

  if (seg1change){
    if (seg1inv) {
      sevenSegWriteInverted(seg1val,22);
    } 
    else {
      sevenSegWrite(seg1val,22);
    }
    seg1change = false;
  }

  if (seg2change){
    if (seg2inv) {
      sevenSegWriteInverted(seg2val,30);
    } 
    else {
      sevenSegWrite(seg2val,30);
    }
    seg2change = false;
  }
  if (seg3change){
    if (seg3inv) {
      sevenSegWriteInverted(seg3val,38);
    } 
    else {
      sevenSegWrite(seg3val,38);
    }
    seg3change = false;
  }
  if (seg4change){
    if (seg4inv) {
      sevenSegWriteInverted(seg4val,46);
    } 
    else {
      sevenSegWrite(seg4val,46);
    }
    seg4change = false;
  }


}

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


void serialEvent1() {
  while (Serial1.available()) {
    char inChar = (char)Serial1.read();
    Serial1.print(inChar);


    int number = inChar - '0';
    if (inChar == '/'){
      number = -1;
    }


    if (number < 10 && number >= -1){
      if (currentaddr == seg1addr){
        seg1val = number;
        seg1change = true;
      }
      if (currentaddr == seg2addr){ 
        seg2val = number;
        seg2change = true;
      }
      if (currentaddr == seg3addr){
        seg3val = number;
        seg3change = true;
      }
      if (currentaddr == seg4addr){
        seg4val = number;
        seg4change = true;
      }
      currentaddr = 'Z';
    }

    currentaddr = inChar;

  }
  serialRX = true;
}

