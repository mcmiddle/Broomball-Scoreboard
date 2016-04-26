#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x44);
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x45);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x47);
Adafruit_PWMServoDriver pwm3 = Adafruit_PWMServoDriver(0x48);
Adafruit_PWMServoDriver pwm4 = Adafruit_PWMServoDriver(0x49);
Adafruit_PWMServoDriver pwm5 = Adafruit_PWMServoDriver(0x4A);
Adafruit_PWMServoDriver pwm6 = Adafruit_PWMServoDriver(0x4B);
Adafruit_PWMServoDriver pwm7 = Adafruit_PWMServoDriver(0x4C);
Adafruit_PWMServoDriver pwm8 = Adafruit_PWMServoDriver(0x4D);
Adafruit_PWMServoDriver pwm9 = Adafruit_PWMServoDriver(0x4E);
Adafruit_PWMServoDriver pwm10 = Adafruit_PWMServoDriver(0x4F);
Adafruit_PWMServoDriver pwm11 = Adafruit_PWMServoDriver(0x50);
Adafruit_PWMServoDriver pwm12 = Adafruit_PWMServoDriver(0x51);
Adafruit_PWMServoDriver pwm13 = Adafruit_PWMServoDriver(0x40);
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
//#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
//#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
//uint8_t servonum = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(100);  // update speed set to 100 Hz
  pwm1.begin();
  
  pwm1.setPWMFreq(100);  // update speed set to 100 Hz
    pwm2.begin();
  
  pwm2.setPWMFreq(100);  // update speed set to 100 Hz
      pwm3.begin();
  
  pwm3.setPWMFreq(100);  // update speed set to 100 Hz
      pwm4.begin();
  
  pwm4.setPWMFreq(100);  // update speed set to 100 Hz
      pwm5.begin();
  
  pwm5.setPWMFreq(100);  // update speed set to 100 Hz
      pwm6.begin();
  
  pwm6.setPWMFreq(100);  // update speed set to 100 Hz
      pwm7.begin();
  
  pwm7.setPWMFreq(100);  // update speed set to 100 Hz
      pwm8.begin();
  
  pwm8.setPWMFreq(100);  // update speed set to 100 Hz
      pwm9.begin();
  
  pwm9.setPWMFreq(100);  // update speed set to 100 Hz
      pwm10.begin();
  
  pwm10.setPWMFreq(100);  // update speed set to 100 Hz
      pwm11.begin();
  
  pwm11.setPWMFreq(100);  // update speed set to 100 Hz
  
      pwm12.begin();
  
  pwm12.setPWMFreq(100);  // update speed set to 100 Hz
  
      pwm13.begin();
  
  pwm13.setPWMFreq(100);  // update speed set to 100 Hz
}


void loop() {
  // Drive each servo one at a time
  Serial.println("in loop");
  for (uint16_t level = 0; level < 4000; level = level + 500){  
  for (uint16_t pulselen = 0; pulselen <= 15 ; pulselen++) {
  pwm.setPin(pulselen, level, 0);
    pwm1.setPin(pulselen, level, 0);
    pwm2.setPin(pulselen, level, 0);
    pwm3.setPin(pulselen, level, 0);
    pwm4.setPin(pulselen, level, 0);
    pwm5.setPin(pulselen, level, 0);
    pwm6.setPin(pulselen, level, 0);
    pwm7.setPin(pulselen, level, 0);
    pwm8.setPin(pulselen, level, 0);
    pwm9.setPin(pulselen, level, 0);
    pwm10.setPin(pulselen, level, 0);
    pwm11.setPin(pulselen, level, 0);
    pwm12.setPin(pulselen, level, 0);
    pwm13.setPin(pulselen, level, 0);
  }
  Serial.println(level);
  delay(1000);
}
 
}
