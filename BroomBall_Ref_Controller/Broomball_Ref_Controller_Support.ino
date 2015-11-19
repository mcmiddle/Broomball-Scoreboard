void readPin(byte pinValues[]){

  if(digitalRead(pinValues[0])==LOW){
    if(!pinValues[2]){
      pinValues[1] = 1;
      pinValues[2] = 1;
      buttonPressed = true;
    } 
    else {
      pinValues[1] = 0;
    }
  } 
  else {
    pinValues[1] = 0;
    pinValues[2] = 0;
  }

}

void printScore(){
  byte homeA = homeScore/10;
  byte homeB = homeScore%10;
  byte visitA = visitScore/10;
  byte visitB = visitScore%10;

  Serial.print("H");
  if(homeA==0){
    Serial.print('/');
  }
  else{
    Serial.print(homeA);
  }

  Serial.print("G");
  Serial.print(homeB);

  Serial.print("B");
  if(visitA==0){
    Serial.print('/');
  }
  else{
    Serial.print(visitA);
  }

  Serial.print("A");
  Serial.print(visitB);
}

void printTime(){

  byte A = time/600;
  byte B = (time/60)%10;
  byte C = (time%60)/10;
  byte D = (time%60)%10;

  gameMatrix.clear();
  Serial.print("F");
  if (A == 0){
    Serial.print('/');
  } 
  else {
    Serial.print(A);
    gameMatrix.writeDigitNum(0, A);
  }
  Serial.print("E");
  if (A == 0 && B == 0){
    Serial.print('/');
  } 
  else {
    Serial.print(B);
    gameMatrix.writeDigitNum(1, B);
  }
  Serial.print("D");
  Serial.print(C);
  Serial.print("C");
  Serial.print(D);
  gameMatrix.drawColon(true);
  gameMatrix.writeDigitNum(3, C);
  gameMatrix.writeDigitNum(4, D);  

  gameMatrix.writeDisplay();
}
void printPenalty(){

  for(byte i = 0; i <= 5; i++){
    if (i!=2 && i!=5){
      if (penalty[i][0]){
        Serial.print(penaltyKey[i][0]); 
        Serial.print((penalty[i][1]/60)%10);
        Serial.print(penaltyKey[i][1]); 
        Serial.print((penalty[i][1]%60)/10);
        Serial.print(penaltyKey[i][2]); 
        Serial.print((penalty[i][1]%60)%10);
      } 
      else {
        for(byte j = 0; j <= 2; j++) {
          Serial.print(penaltyKey[i][j]); 
          Serial.print('/');
        }
      }
    }
  }
}
void countDownPenalty(){
  boolean penaltyCheck = false;
  for(byte count = 0; count <= 5; count++){
    if (penalty[count][0]){
      penalty[count][1] = penalty[count][1] - 1;
      if (penalty[count][1] == 0){
        penalty[count][0] = 0;
        if (count == 0 || count == 1 || count == 3 || count == 4){
          if (penalty[count+1][0] == 1 && penalty[count+1][1] != 1){
            penalty[count][0] = 1;
            penalty[count][1] = penalty[count+1][1];
            penalty[count+1][0] = 0;
            penalty[count+1][1] = 0;
            if (count == 1 || count == 4){
              if (penalty[count-1][0] == 0){
                penalty[count-1][0] = 1;
                penalty[count-1][1] = penalty[count][1];
                penalty[count][0] = 0;
                penalty[count][1] = 0;
              }
            }
          }
          if (penalty[count+2][0] == 1 && penalty[count+2][1] != 1){
            penalty[count+1][0] = 1;
            penalty[count+1][1] = penalty[count+2][1];
            penalty[count+2][0] = 0;
            penalty[count+2][1] = 0;
          }
        }
      }
      penaltyCheck = true;
    }
  }
  if (penaltyCheck){
    printPenalty();
  }
}

void addHomePenalty(int time){
  if(penalty[0][0] == 1){
    if(penalty[1][0] == 1){  
      if(penalty[2][0] != 1){
        penalty[2][0] = 1;
        penalty[2][1] = time;  
      }
    } 
    else {
      penalty[1][0] = 1;
      penalty[1][1] = time;  
    }
  } 
  else {
    penalty[0][0] = 1;
    penalty[0][1] = time;
  }
}
void addvisitPenalty(int time){
  if(penalty[3][0] == 1){
    if(penalty[4][0] == 1){  
      if(penalty[5][0] != 1){
        penalty[5][0] = 1;
        penalty[5][1] = time;  
      }
    } 
    else {
      penalty[4][0] = 1;
      penalty[4][1] = time;  
    }
  } 
  else {
    penalty[3][0] = 1;
    penalty[3][1] = time;
  }
}

void readPins(){

  buttonPressed = false;
  readPin(homeScoreUp);
  readPin(homeScoreDown);
  readPin(visitScoreUp);
  readPin(visitScoreDown);
  readPin(startTime);
  readPin(stopTime);
  readPin(timePlus);
  readPin(timeMinus);
  readPin(homeMinor);
  readPin(homeMajor);
  readPin(visitMinor);
  readPin(visitMajor);

}

void debug(){

  if(changingTime){

    Serial.print(changeGameClock/600);
    Serial.print((changeGameClock/60)%10);
    Serial.print(':');
    Serial.print((changeGameClock%60)/10);
    Serial.print((changeGameClock%60)%10);
    Serial.print(' ');
    Serial.print(changeGlobalClock/600);
    Serial.print((changeGlobalClock/60)%10);
    Serial.print(':');
    Serial.print((changeGlobalClock%60)/10);
    Serial.println((changeGlobalClock%60)%10);

  } 
  else {

    Serial.println();
    Serial.println("-----------------------");
    Serial.print(homeScore);
    Serial.print(' ');
    Serial.print(time/600);
    Serial.print((time/60)%10);
    Serial.print(':');
    Serial.print((time%60)/10);
    Serial.print((time%60)%10);
    Serial.print(' ');
    Serial.println(visitScore);

    int j = 0;
    Serial.print((penalty[j][1]/60)%10);
    Serial.print(':');
    Serial.print((penalty[j][1]%60)/10);
    Serial.print((penalty[j][1]%60)%10);
    Serial.print(' ');
    Serial.print(period);
    j = 3;
    Serial.print(' ');
    Serial.print((penalty[j][1]/60)%10);
    Serial.print(':');
    Serial.print((penalty[j][1]%60)/10);
    Serial.println((penalty[j][1]%60)%10);
    j = 1;
    Serial.print((penalty[j][1]/60)%10);
    Serial.print(':');
    Serial.print((penalty[j][1]%60)/10);
    Serial.print((penalty[j][1]%60)%10);
    Serial.print("   ");
    j = 4;
    Serial.print((penalty[j][1]/60)%10);
    Serial.print(':');
    Serial.print((penalty[j][1]%60)/10);
    Serial.println((penalty[j][1]%60)%10); 
    j = 2;
    Serial.print((penalty[j][1]/60)%10);
    Serial.print(':');
    Serial.print((penalty[j][1]%60)/10);
    Serial.print((penalty[j][1]%60)%10);
    Serial.print("---");
    j = 5;
    Serial.print((penalty[j][1]/60)%10);
    Serial.print(':');
    Serial.print((penalty[j][1]%60)/10);
    Serial.println((penalty[j][1]%60)%10); 

    Serial.print(temperature);
    Serial.print("  ");
    if(outputTime/600 != 0){
      Serial.print((outputTime/600));
    }
    Serial.print((outputTime/60)%10);
    Serial.print(':');
    Serial.print((outputTime%60)/10);
    Serial.println((outputTime%60)%10);
    Serial.println("-----------------------");
  }

}

void triggerHorn(unsigned int duration){
  int afterEvent = horn.after(duration, turnOffHorn);
  Serial.print("V8");
}

void turnOffHorn(){
  Serial.print("V/");
}

void setPeriod(){
  Serial.print("U");
  Serial.print(period);
}

void resetGame(){
  for(int i = 0; i <= 5; i++){
    for(int j = 0; j <= 1; j++){
      penalty[i][j] = 0;
    }  
  }
  changingTime = false;
  updateBlink.stop(blinkEvent);
  timeActive = false;
  time = 900; //15 Min
  period = 1;
  switchPeriod = 0;
  homeScore = 0;
  visitScore = 0;
  printTime();
  printPenalty();
  printScore();
  setPeriod();


}

void updateGlobalClock(){
  if (!changingTime){

    if (tempDisplay <= 5){
      bool h12;
      bool PM;
      int receivedTime = (60*Clock.getHour(h12,PM))+(Clock.getMinute());

      if (receivedTime <= 1440){
        globalTime = receivedTime;
      } 
      else {
      }

      outputTime = globalTime;
      if (globalTime >= 780){
        outputTime = globalTime - 720;
      }

      byte A = (outputTime/600);
      byte B = (outputTime/60)%10;
      byte C = (outputTime%60)/10;
      byte D = (outputTime%60)%10;

      globalMatrix.clear();
      if (A != 0){
        globalMatrix.writeDigitNum(0, A);
      }
      globalMatrix.writeDigitNum(1, B);
      globalMatrix.drawColon(true);
      globalMatrix.writeDigitNum(3, C);
      globalMatrix.writeDigitNum(4, D);  
      globalMatrix.writeDisplay();
      tempDisplay++;
    } 
    else {
      temperature=(((Clock.getTemperature())*9)/5)+32;
      if (temperature != 490){
        globalMatrix.clear();
        globalMatrix.drawColon(false);
        globalMatrix.print(temperature*10, DEC);
        globalMatrix.writeDigitNum(4, 15);
        globalMatrix.writeDisplay();
      } 
      else {
        tempDisplay=0;
      }
      if (tempDisplay >= 7){
        tempDisplay=0;
      }
      tempDisplay++;
    }
  }
}

void changeTimeBlink() {
  if(changingTime){
    blinkOn = !blinkOn;
    if(changingGlobalClock && blinkOn){
      printToMatrix(0,0,changeGlobalClock); 
    } 
    else {
      printToMatrix(1,0,changeGlobalClock);
    }
    if(!changingGlobalClock && blinkOn){
      printToMatrix(0,1,changeGameClock);
    } 
    else {
      printToMatrix(1,1,changeGameClock);
    }
  }
}

void printToMatrix(byte on, byte sevenseg, unsigned int methodTime){

  byte A = (methodTime/600);
  byte B = (methodTime/60)%10;
  byte C = (methodTime%60)/10;
  byte D = (methodTime%60)%10;

  if (sevenseg){
    if (on) {
      gameMatrix.clear();
      if (A != 0){
        gameMatrix.writeDigitNum(0, A);
      }
      gameMatrix.writeDigitNum(1, B);
      gameMatrix.drawColon(true);
      gameMatrix.writeDigitNum(3, C);
      gameMatrix.writeDigitNum(4, D);
      gameMatrix.writeDisplay();
    } 
    else {
      gameMatrix.clear();
      gameMatrix.writeDisplay();
    }
  } 
  else {    
    if (on) {
      globalMatrix.clear();
      if (A != 0){
        globalMatrix.writeDigitNum(0, A);
      }
      globalMatrix.writeDigitNum(1, B);
      globalMatrix.drawColon(true);
      globalMatrix.writeDigitNum(3, C);
      globalMatrix.writeDigitNum(4, D);
      globalMatrix.writeDisplay();
    } 
    else {
      globalMatrix.clear();
      globalMatrix.writeDisplay();
    }
  }
}

void countDown(){

  if (time == 1){
    time = 0;
    timeActive = false;
    printTime();
    countDownPenalty();
    switchPeriod = 5;
    triggerHorn(3000);
  } 
  else {  
    if(switchPeriod == 0 && time!=0){
      time = time - 1;
      printTime();
      countDownPenalty();
    }
  }

  if (time == 0){
    if (switchPeriod == 1){
      switchPeriod = 0;
      t.stop(tickEvent);
      if (period == 2 && homeScore == visitScore){
        time = 300;
        printTime();
        period = 3;
        setPeriod();
      }
      if (period == 1){
        time = 900;
        printTime();
        period = 2;
        setPeriod();
      }
    } 
    else {
      switchPeriod--;  
    }  
  } 

  if (time == 300 || time == 60){
    if(period == 1 || period == 2){
      triggerHorn(500);
    }
    if(time == 60 && period == 3){
      triggerHorn(500);
    }
  } 

  if (debugOnValue){
    debug();
  }
}

void removePenalty(byte side, byte row){
  byte offset = 0;
  if(side){
    offset=3;
  }
  if(row == 0){
    if(penalty[0+offset][0]){
      penalty[0+offset][0] = 0;
      penalty[0+offset][1] = 0;
      if(penalty[1+offset][0] == 1){
        penalty[0+offset][0] = 1;
        penalty[0+offset][1] = penalty[1+offset][1];
        penalty[1+offset][0] = 0;
        penalty[1+offset][1] = 0;  
      }
      if(penalty[2+offset][0] == 1){
        penalty[1+offset][0] = 1;
        penalty[1+offset][1] = penalty[2+offset][1];
        penalty[2+offset][0] = 0;
        penalty[2+offset][1] = 0;  
      }
    }
  }
  if(row == 1){
    if(penalty[1+offset][0]){
      penalty[1+offset][0] = 0;
      penalty[1+offset][1] = 0;
      if(penalty[2+offset][0] == 1){
        penalty[1+offset][0] = 1;
        penalty[1+offset][1] = penalty[2+offset][1];
        penalty[2+offset][0] = 0;
        penalty[2+offset][1] = 0;  
      }
    }
  }
}


