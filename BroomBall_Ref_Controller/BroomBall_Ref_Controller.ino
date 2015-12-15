#include <Wire.h>
#include <DS3231.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
Adafruit_7segment gameMatrix = Adafruit_7segment();
Adafruit_7segment globalMatrix = Adafruit_7segment();
#include "Event.h"
#include "Timer.h"

Timer t;
Timer horn;
Timer global;
Timer updateBlink;
DS3231 Clock;

boolean debugOnValue = false;

// [pin#, pressed, active]
static byte homeScoreUp[3] = {
  13, 0, 0};
static byte homeScoreDown[3] = {
  12, 0, 0};
static byte visitScoreUp[3] = {
  7, 0, 0};
static byte visitScoreDown[3] = {
  6, 0, 0};
static byte startTime[3] = {
  4, 0, 0};
static byte stopTime[3] = {
  5, 0, 0};
static byte timePlus[3] = {
  2, 0, 0};
static byte timeMinus[3] = {
  3, 0, 0};  
static byte homeMinor[3] = {
  10, 0, 0};   
static byte homeMajor[3] = {
  11, 0, 0};  
static byte visitMinor[3] = {
  8, 0, 0};   
static byte visitMajor[3] = {
  9, 0, 0}; 

boolean buttonPressed = false;

unsigned int time = 900; //15 Min = 900
unsigned int globalTime = 0;
unsigned int outputTime = 0;
int temperature = 0;
byte tempDisplay = 0;
byte period = 1;
byte switchPeriod = 0;

boolean changingTime = false;
boolean changingGlobalClock = false;
unsigned int changeGameClock = 0;
unsigned int changeGlobalClock = 0;
boolean blinkOn = false;

boolean timeActive = false;

unsigned int penalty[6][2] = {
  {
    0,0                      }
  ,{
    0,0                      }
  ,{
    0,0                      }
  ,{
    0,0                      }
  ,{
    0,0                      }
  ,{
    0,0                      }
};

char penaltyKey[6][3] = {
  {
    'N','M','L'                  }
  ,{
    'T','S','R'                  }
  ,{
    0,0,0                 }
  ,{
    'K','J','I'                  }
  ,{
    'Q','P','O'                  }
  ,{
    0,0,0                  }
};



byte homeScore = 0;
byte visitScore = 0;

int tickEvent = t.every(1000, countDown);
int globalEvent = global.every(1000, updateGlobalClock);
int blinkEvent = updateBlink.every(500, changeTimeBlink);


void setup() {
  updateBlink.stop(blinkEvent);
  t.stop(tickEvent);

  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);
  
  delay(3000);  // Crutial delay before writing to the Xbee, otherwise xbee will not function properly.
  
  Serial.begin(9600);
  gameMatrix.begin(0x75);
  globalMatrix.begin(0x73);
  for(int count = 0; count <= 13; count++){
    if (count != 1){
      pinMode(count, INPUT_PULLUP);
    }
  }
  //Serial.print("WAKE UP!");
  delay(1000);
  //Wake everything up
  for(byte i = 0; i <= 3; i++){
    for(char j = 'A'; j <= 'U'; j++){
      Serial.print(j);
      if (i <= 1){
        Serial.print(8);
      } 
      else {
        Serial.print('/');
      }
    }
    delay (1000);
  }
  printTime();
  printPenalty();
  printScore();
  setPeriod();
  turnOffHorn();
}

void loop() {

  readPins();

  if(changingTime){
    changeClocks();    
  } 
  else {

    if(startTime[1] && !stopTime[2]){
      if (!timeActive && switchPeriod == 0 && time!=0){
        tickEvent = t.every(1000, countDown);
        timeActive = true;
      }
      if (time == 0 && switchPeriod == 0 && period == 2){
        time = 300;
        printTime();
        period = 3;
        setPeriod();
      }

    }
    if(stopTime[1]){
      if (timeActive){
        t.stop(tickEvent);
        timeActive = false;
      }
    }
    if(visitScoreUp[1]){ 
      if(visitScore != 99){
        visitScore ++;
        printScore();
      }
    }
    if(visitScoreDown[1]){
      if (visitScore != 0){
        visitScore --;
        printScore();
      }
    }
    if(homeScoreUp[1]){ 
      if(homeScore != 99){
        homeScore ++;
        printScore();
      }
    }
    if(homeScoreDown[1]){
      if (homeScore != 0){
        homeScore --;
        printScore();
      }
    }
    if(homeMinor[1]){
      if(stopTime[2]){
        removePenalty(0,0);
        printPenalty();
      } 
      else {
        addHomePenalty(120);
        printPenalty();
      }
    }
    if(homeMajor[1]){
      if(stopTime[2]){
        removePenalty(0,1);
        printPenalty();
      } 
      else {
        addHomePenalty(300);
        printPenalty();
      }
    }
    if(visitMinor[1]){
      if(stopTime[2]){
        removePenalty(1,0);
        printPenalty();
      } 
      else {
        addvisitPenalty(120);
        printPenalty();
      }
    }
    if(visitMajor[1]){
      if(stopTime[2]){
        removePenalty(1,1);
        printPenalty();
      } 
      else {
        addvisitPenalty(300);
        printPenalty();
      }
    }
    if(timePlus[1] || timeMinus[1]){
      if(!timeActive){
        changingTime = true;
        changingGlobalClock = false;
        changeGameClock = time;
        changeGlobalClock = globalTime;
        blinkEvent = updateBlink.every(250, changeTimeBlink);
      }
    }
  }

  if(startTime[2] && stopTime[1] || stopTime[2] && startTime[1]){
    resetGame();
  }

  if (debugOnValue && buttonPressed){
    debug();
  }

  t.update();
  horn.update();
  updateBlink.update();
  global.update();
  delay(4);
}

void changeClocks() {

  if(startTime[1]){
    changingTime = false;
    updateBlink.stop(blinkEvent);
    time = changeGameClock;
    if(globalTime != changeGlobalClock){
      globalTime = changeGlobalClock;
      Clock.setSecond(0);//Set the second 
      Clock.setMinute(changeGlobalClock%60);//Set the minute 
      Clock.setHour(changeGlobalClock/60);  //Set the hour 
    }
    printTime();
  }
  if(stopTime[1]){
    changingTime = false;
    updateBlink.stop(blinkEvent);
    printTime();
  }
  if(visitScoreUp[1]){ 
    if(changingGlobalClock && changeGlobalClock <= 1439){
      changeGlobalClock = changeGlobalClock + 1;
    } 
    else {
      if (changeGameClock <= 1799) {
        changeGameClock = changeGameClock + 1;
      }
    }
  }
  if(visitScoreDown[1]){
    if(changingGlobalClock && changeGlobalClock >= 1){
      changeGlobalClock = changeGlobalClock - 1;
    } 
    else {
      if (changeGameClock >= 1) {
        changeGameClock = changeGameClock - 1;
      }
    }
  }
  if(homeScoreUp[1]){ 
    if(changingGlobalClock && changeGlobalClock <= 1380){
      changeGlobalClock = changeGlobalClock + 60;
    } 
    else {
      if (changeGameClock <= 1740) {
        changeGameClock = changeGameClock + 60;
      }
    }
  }
  if(homeScoreDown[1]){
    if(changingGlobalClock && changeGlobalClock >= 60){
      changeGlobalClock = changeGlobalClock - 60;
    } 
    else {
      if (changeGameClock >= 60) {
        changeGameClock = changeGameClock - 60;
      }
    }
  }
  if(visitMinor[1]){
    if(changingGlobalClock && changeGlobalClock <= 1430){
      changeGlobalClock = changeGlobalClock + 10;
    } 
    else {
      if (changeGameClock <= 1790) {
        changeGameClock = changeGameClock + 10;
      }
    }
  }
  if(visitMajor[1]){
    if(changingGlobalClock && changeGlobalClock >= 10){
      changeGlobalClock = changeGlobalClock - 10;
    } 
    else {
      if (changeGameClock >= 10) {
        changeGameClock = changeGameClock - 10;
      }
    }
  }
  if(homeMinor[1]){
    if(changingGlobalClock){
      changeGlobalClock = 720;
    } 
    else {
      changeGameClock = 900;
    }
  }
  if(homeMajor[1]){
    if(changingGlobalClock){
      changeGlobalClock = 1440;
    } 
    else {
      changeGameClock = 1800;
    }
  }
  if(timePlus[1] || timeMinus[1]){
    changingGlobalClock = !changingGlobalClock;
  }
  if (buttonPressed){
    changeTimeBlink();
    blinkOn = true;
  }

}














