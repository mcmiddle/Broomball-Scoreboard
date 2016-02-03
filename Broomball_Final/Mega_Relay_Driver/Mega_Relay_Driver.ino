//Code written by Jordan Grider, Michael Middleton, and Tino Moore for an Arduino Mega to control 7-segment output for a relay board
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <QueueArray.h>

int brightness = 4000; 
QueueArray <char> serialQueue;

//Consists of 1's and 0's that map to a segment of the display
//Time, Penalties, and Score 10s digit: decodeType = 2 for normal and 3 for inverted. Pattern is XBACGFED
//Time and Score 1s digit: decodeType = 1 for normal and 4 for inverted. Pattern is XDCBGAFE
//Penalty 1s digit: decodeType = 5. Pattern is XCBDGAFE
//Period: decodeType = 6. Pattern is XBAFGCED
//Penalty Minutes: decodeType = 0. Pattern is XABFGCED
// X is don't care value for above

//char currentAddr = 'Z';
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

int bank[21][4] = {
  //start pin, decode type, dot pin, pwm id
  /*A*/{8,1,-1,1},
  /*B*/{1,2,-1,1},
  /*C*/{1,3,-1,4},
  /*D*/{8,4,15,4},
  /*E*/{8,1,15,3},
  /*F*/{1,2,-1,3},
  /*G*/{8,1,-1,0},
  /*H*/{1,2,-1,0},
  /*I*/{8,5,-1,10},
  /*J*/{1,2,-1,10},
  /*K*/{1,0,8,9},
  /*L*/{8,5,-1,6},
  /*M*/{1,2,-1,6},
  /*N*/{1,0,8,5},
  /*O*/{8,5,-1,12},
  /*P*/{1,2,-1,12},
  /*Q*/{1,0,8,11},
  /*R*/{8,5,-1,8},
  /*S*/{1,2,-1,8},
  /*T*/{1,0,8,7},
  /*U*/{1,6,-1,2}
};

byte seven_seg_digits[7][11][7] = {                           
  //type 0
  {
  { 
    1,1,1,0,1,1,1   }
  ,  // = 0
  { 
    0,1,0,0,1,0,0   }
  ,  // = 1
  { 
    1,1,0,1,0,1,1   }
  ,  // = 2
  { 
    1,1,0,1,1,0,1   }
  ,  // = 3
  { 
    0,1,1,1,1,0,0   }
  ,  // = 4
  { 
    1,0,1,1,1,0,1   }
  ,  // = 5
  { 
    1,0,1,1,1,1,1   }
  ,  // = 6
  { 
    1,1,0,0,1,0,0   }
  ,  // = 7
  { 
    1,1,1,1,1,1,1   }
  ,  // = 8
  { 
    1,1,1,1,1,0,0   }
  ,   // = 9
  { 
    0,0,0,0,0,0,0   } 
  },
  //type 1
  {                  
  { 
    1,1,1,0,1,1,1   }
  ,  // = 0
  { 
    0,1,1,0,0,0,0   }
  ,  // = 1
  { 
    1,0,1,1,1,0,1   }
  ,  // = 2
  { 
    1,1,1,1,1,0,0   }
  ,  // = 3
  { 
    0,1,1,1,0,1,0   }
  ,  // = 4
  { 
    1,1,0,1,1,1,0   }
  ,  // = 5
  { 
    1,1,0,1,1,1,1   }
  ,  // = 6
  { 
    0,1,1,0,1,0,0   }
  ,  // = 7
  { 
    1,1,1,1,1,1,1   }
  ,  // = 8
  { 
    0,1,1,1,1,1,0   }
  ,   // = 9
  { 
    0,0,0,0,0,0,0   } 
  },
  //type 2
  {                           
  { 
    1,1,1,0,1,1,1   }
  ,  // = 0
  { 
    1,0,1,0,0,0,0   }
  ,  // = 1
  { 
    1,1,0,1,0,1,1   }
  ,  // = 2
  { 
    1,1,1,1,0,0,1   }
  ,  // = 3
  { 
    1,0,1,1,1,0,0   }
  ,  // = 4
  { 
    0,1,1,1,1,0,1   }
  ,  // = 5
  { 
    0,1,1,1,1,1,1   }
  ,  // = 6
  { 
    1,1,1,0,0,0,0   }
  ,  // = 7
  { 
    1,1,1,1,1,1,1   }
  ,  // = 8
  { 
    1,1,1,1,1,0,0   }
  ,   // = 9
  { 
    0,0,0,0,0,0,0   } 
  },
  //type 3
  {                           
  { 
    1,1,1,0,1,1,1   }
  ,  // = 0
  { 
    0,0,0,0,1,1,0   }
  ,  // = 1
  { 
    1,1,0,1,0,1,1   }
  ,  // = 2
  { 
    0,1,0,1,1,1,1   }
  ,  // = 3
  { 
    0,0,1,1,1,1,0   }
  ,  // = 4
  { 
    0,1,1,1,1,0,1   }
  ,  // = 5
  { 
    1,1,1,1,1,0,1  }
  ,  // = 6
  { 
    0,0,0,0,1,1,1   }
  ,  // = 7
  { 
    1,1,1,1,1,1,1   }
  ,  // = 8
  { 
    0,0,1,1,1,1,1   }
  ,   // = 9
  { 
    0,0,0,0,0,0,0   } 
  },
  //type 4
  {                           
  { 
    1,1,1,0,1,1,1   }
  ,  // = 0
  { 
    0,0,0,0,0,1,1   }
  ,  // = 1
  { 
    1,0,1,1,1,0,1   }
  ,  // = 2
  { 
    1,0,0,1,1,1,1   }
  ,  // = 3
  { 
    0,1,0,1,0,1,1   }
  ,  // = 4
  { 
    1,1,0,1,1,1,0   }
  ,  // = 5
  { 
    1,1,1,1,1,1,0   }
  ,  // = 6
  { 
    1,0,0,0,0,1,1   }
  ,  // = 7
  { 
    1,1,1,1,1,1,1   }
  ,  // = 8
  { 
    1,1,0,1,1,1,0   }
  ,   // = 9
  { 
    0,0,0,0,0,0   } 
  },
  //type 5
  {                  
  { 
    1,1,1,0,1,1,1   }
  ,  // = 0
  { 
    1,1,0,0,0,0,0   }
  ,  // = 1
  { 
    0,1,1,1,1,0,1   }
  ,  // = 2
  { 
    1,1,1,1,1,0,0   }
  ,  // = 3
  { 
    1,1,0,1,0,1,0   }
  ,  // = 4
  { 
    1,0,1,1,1,1,0   }
  ,  // = 5
  { 
    1,0,1,1,1,1,1   }
  ,  // = 6
  { 
    1,1,0,0,1,0,0   }
  ,  // = 7
  { 
    1,1,1,1,1,1,1   }
  ,  // = 8
  { 
    1,1,0,1,1,1,0   }
  ,   // = 9
  { 
    0,0,0,0,0,0,0   } 
  },
  //type 6
  {                           
  { 
    1,1,1,0,1,1,1   }
  ,  // = 0
  { 
    1,0,0,0,1,0,0   }
  ,  // = 1
  { 
    1,1,0,1,0,1,1   }
  ,  // = 2
  { 
    1,1,0,1,1,0,1   }
  ,  // = 3
  { 
    0,1,1,1,1,0,0   }
  ,  // = 4
  { 
    1,0,1,1,1,0,1  }
  ,  // = 5
  { 
    1,0,1,1,1,1,1   }
  ,  // = 6
  { 
    1,1,0,0,1,0,0   }
  ,  // = 7
  { 
    1,1,1,1,1,1,1   }
  ,  // = 8
  { 
    1,1,1,1,1,0,0   }
  ,   // = 9
  { 
    0,0,0,0,0,0,0   } 
  }
};




void setup() {                

  Wire.begin();
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
    char addr, value,startPin, pwmID, dotPin, decodeType,index;
    int i, j;
 
  //Display update logic
  if (serialQueue.count() >= 2){
   addr = serialQueue.dequeue();
         value = serialQueue.dequeue();
        
         if (addr == 'Y'){
          //brightness stuff  
          if (value == 1){brightness = 100;}
          else if (value == 2){brightness = 200;}
          else if (value == 3){brightness = 300;}
          else if (value == 4){brightness = 400;}
          else {brightness = (value - 4)*500;}
          
        
         } else /*display update*/ {
          index = addr - 'A';
          
          startPin =  bank[index][0];
          decodeType = bank[index][1];
          dotPin = bank[index][2];
          pwmID = bank[index][3];
          
          if (value == -1){
            if (addr == 'K'){
              pwmAwayPenaltyTopMinutes.setPin(8, 0, 0);
              dotPin = -1;
            }
            else if (addr == 'N'){
              pwmHomePenaltyTopMinutes.setPin(8, 0, 0);
              dotPin = -1;
            } else if (addr == 'Q'){
              pwmAwayPenaltyBottomMinutes.setPin(8, 0, 0);
              dotPin = -1;
            }else if (addr == 'T'){
              pwmHomePenaltyBottomMinutes.setPin(8, 0, 0);
              dotPin = -1;
            }
          }

          
          switch(pwmID){
             case 0 :
                 //Write the value to the display
                  j = 6;
              for (i = startPin; i < startPin + 7; i ++){  
                    if (seven_seg_digits[decodeType][value][j] == 1){
                      pwmHomeScore.setPin(i, brightness, 0);  //turn segment on
                    } else {
                      pwmHomeScore.setPin(i, 0, 0);  //turn segment off
                    }
                    j--;
                  }
                  
                  if (dotPin != -1)
                    pwmHomeScore.setPin(i, brightness, 0);  //turn segment on
                  else
                    
                break;
                
             case 1 :
                //Write the value to the display
                  j = 6;
              for (i = startPin; i < startPin + 7; i ++){  
                    if (seven_seg_digits[decodeType][value][j] == 1){
                      pwmVisitorScore.setPin(i, brightness, 0);  //turn segment on
                    } else {
                      pwmVisitorScore.setPin(i, 0, 0);  //turn segment off
                    }
                    j--;
                  }
                  
                  if (dotPin != -1)
                    pwmVisitorScore.setPin(i, brightness, 0);  //turn segment on
                    
                break;
                
             case 2 :
               //Write the value to the display
                  j = 6;
              for (i = startPin; i < startPin + 7; i ++){  
                    if (seven_seg_digits[decodeType][value][j] == 1){
                      pwmPeriod.setPin(i, brightness, 0);  //turn segment on
                    } else {
                      pwmPeriod.setPin(i, 0, 0);  //turn segment off
                    }
                    j--;
                  }
                  
                  if (dotPin != -1)
                    pwmPeriod.setPin(i, brightness, 0);  //turn segment on
                break;
                
             case 3 :
               //Write the value to the display
                  j = 6;
              for (i = startPin; i < startPin + 7; i ++){  
                    if (seven_seg_digits[decodeType][value][j] == 1){
                      pwmTimeMinutes.setPin(i, brightness, 0);  //turn segment on
                    } else {
                      pwmTimeMinutes.setPin(i, 0, 0);  //turn segment off
                    }
                    j--;
                  }
                  
                  if (dotPin != -1)
                    pwmTimeMinutes.setPin(i, brightness, 0);  //turn segment on
                break;
             case 4 :
               //Write the value to the display

                  j = 6;
              for (i = startPin; i < startPin + 7; i ++){  
                    if (seven_seg_digits[decodeType][value][j] == 1){
                      pwmTimeSeconds.setPin(i, brightness, 0);  //turn segment on
                    } else {
                      pwmTimeSeconds.setPin(i, 0, 0);  //turn segment off
                    }
                    j--;
                  }
                  
                  if (dotPin != -1)
                    pwmTimeSeconds.setPin(i, brightness, 0);  //turn segment on
                break;
             case 5 :
               //Write the value to the display
                  j = 6;
              for (i = startPin; i < startPin + 7; i ++){  
                    if (seven_seg_digits[decodeType][value][j] == 1){
                      pwmHomePenaltyTopMinutes.setPin(i, brightness, 0);  //turn segment on
                    } else {
                      pwmHomePenaltyTopMinutes.setPin(i, 0, 0);  //turn segment off
                    }
                    j--;
                  }
                  
                  if (dotPin != -1)
                    pwmHomePenaltyTopMinutes.setPin(i, brightness, 0);  //turn segment on
                break;
             case 6 :
               //Write the value to the display
                  j = 6;
              for (i = startPin; i < startPin + 7; i ++){  
                    if (seven_seg_digits[decodeType][value][j] == 1){
                      pwmHomePenaltyTopSeconds.setPin(i, brightness, 0);  //turn segment on
                    } else {
                      pwmHomePenaltyTopSeconds.setPin(i, 0, 0);  //turn segment off
                    }
                    j--;
                  }
                  
                  if (dotPin != -1)
                    pwmHomePenaltyTopSeconds.setPin(i, brightness, 0);  //turn segment on
                break;
             case 7 :
               //Write the value to the display
                  j = 6;
              for (i = startPin; i < startPin + 7; i ++){  
                    if (seven_seg_digits[decodeType][value][j] == 1){
                      pwmHomePenaltyBottomMinutes.setPin(i, brightness, 0);  //turn segment on
                    } else {
                      pwmHomePenaltyBottomMinutes.setPin(i, 0, 0);  //turn segment off
                    }
                    j--;
                  }
                  
                  if (dotPin != -1)
                    pwmHomePenaltyBottomMinutes.setPin(i, brightness, 0);  //turn segment on
                break;
             case 8 :
               //Write the value to the display
                  j = 6;
              for (i = startPin; i < startPin + 7; i ++){  
                    if (seven_seg_digits[decodeType][value][j] == 1){
                      pwmHomePenaltyBottomSeconds.setPin(i, brightness, 0);  //turn segment on
                    } else {
                      pwmHomePenaltyBottomSeconds.setPin(i, 0, 0);  //turn segment off
                    }
                    j--;
                  }
                  
                  if (dotPin != -1)
                    pwmHomePenaltyBottomSeconds.setPin(i, brightness, 0);  //turn segment on
                break;
             case 9 :
               //Write the value to the display
                  j = 6;
              for (i = startPin; i < startPin + 7; i ++){  
                    if (seven_seg_digits[decodeType][value][j] == 1){
                      pwmAwayPenaltyTopMinutes.setPin(i, brightness, 0);  //turn segment on
                    } else {
                      pwmAwayPenaltyTopMinutes.setPin(i, 0, 0);  //turn segment off
                    }
                    j--;
                  }
                  
                  if (dotPin != -1)
                    pwmAwayPenaltyTopMinutes.setPin(i, brightness, 0);  //turn segment on
                break;
             case 10 :
               //Write the value to the display
                  j = 6;
              for (i = startPin; i < startPin + 7; i ++){  
                    if (seven_seg_digits[decodeType][value][j] == 1){
                      pwmAwayPenaltyTopSeconds.setPin(i, brightness, 0);  //turn segment on
                    } else {
                      pwmAwayPenaltyTopSeconds.setPin(i, 0, 0);  //turn segment off
                    }
                    j--;
                  }
                  
                  if (dotPin != -1)
                    pwmAwayPenaltyTopSeconds.setPin(i, brightness, 0);  //turn segment on
                break;
             case 11 :
               //Write the value to the display
                  j = 6;
              for (i = startPin; i < startPin + 7; i ++){  
                    if (seven_seg_digits[decodeType][value][j] == 1){
                      pwmAwayPenaltyBottomMinutes.setPin(i, brightness, 0);  //turn segment on
                    } else {
                      pwmAwayPenaltyBottomMinutes.setPin(i, 0, 0);  //turn segment off
                    }
                    j--;
                  }
                  
                  if (dotPin != -1)
                    pwmAwayPenaltyBottomMinutes.setPin(i, brightness, 0);  //turn segment on
                break;
             case 12 :
               //Write the value to the display
                  j = 6;
              for (i = startPin; i < startPin + 7; i ++){  
                    if (seven_seg_digits[decodeType][value][j] == 1){
                      pwmAwayPenaltyBottomSeconds.setPin(i, brightness, 0);  //turn segment on
                    } else {
                      pwmAwayPenaltyBottomSeconds.setPin(i, 0, 0);  //turn segment off
                    }
                    j--;
                  }
                  
                  if (dotPin != -1)
                    pwmAwayPenaltyBottomSeconds.setPin(i, brightness, 0);  //turn segment on
                break;
          }//end case
        }//end else display update

  }//end if queue avail
  
}   //end loop



void serialEvent() {
  while (Serial.available() >= 2) {        
    
    //Recieve data in on RX and send it out on TX to next Arduino
    char addr = (char)Serial.read();
    char value = (char)Serial.read();
    
    //pass to next arduino in chain
    Serial.print(addr);
    Serial.print(value);
    
    if (addr != 'Y'){
      value = value - '0';
      if (value == '/'){
        value = -1;
      }
    }
    
    //enqueue for use in loop
    serialQueue.enqueue(addr);
    serialQueue.enqueue(value); 
  }
}
