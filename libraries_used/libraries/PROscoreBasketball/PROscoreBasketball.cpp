#include "PROscoreBasketball.h"


PROscoreBasketBall::PROscoreBasketBall() {
}

void PROscoreBasketBall::begin() {

}

void PROscoreBasketBall::loop(){
  if (gametime_run) {
    if (millis() - gt_last_millis >= 5) {
      gametime_millis--;
      if (gametime_millis > 9) {
        gametime_millis = 9;
        gametime_second--;
        if (gametime_second > 59) {
          gametime_second = 59;
          gametime_minute--;
        }
      }
    }
    gt_last_millis=millis();
  }

  if(shotclock_run){
    if(millis()-sc_last_millis>=5){
      shotclock_millis--;
      if(shotclock_millis>9){
        shotclock_millis=9;
        shotclock_second--;
      }
    }
    if(shotclock_second==0 && shotclock_millis==0){
      shotclock_run=false;
    }
  }

}

void PROscoreBasketBall::Start_GameTime(){
	gt_last_millis=millis();
	gametime_run=true;
}

void PROscoreBasketBall::Stop_GameTime(){
	gametime_run=false;
}

bool PROscoreBasketBall::get_GameTimeRun(){
	return gametime_run;
}


uint8_t PROscoreBasketBall::get_GameTime_Minute() {
  return gametime_minute;
}

uint8_t PROscoreBasketBall::get_GameTime_Second() {
  return gametime_second;
}

uint8_t PROscoreBasketBall::get_GameTime_Millis() {
  return gametime_millis;
}

String PROscoreBasketBall::getString_GameTime() {

  char ch_GameTime[8] = "";

  if (gametime_minute < 10) {
    if (gametime_second < 10) {
      sprintf(ch_GameTime, "0%d:0%d.%d", gametime_minute, gametime_second, gametime_millis);
    } else {
      sprintf(ch_GameTime, "0%d:%d.%d", gametime_minute, gametime_second, gametime_millis);
    }
  } else {
    if (gametime_second < 10) {
      sprintf(ch_GameTime, "%d:0%d.%d", gametime_minute, gametime_second, gametime_millis);
    } else {
      sprintf(ch_GameTime, "%d:%d.%d", gametime_minute, gametime_second, gametime_millis);
    }
  }

  return String(ch_GameTime);
 
}

void PROscoreBasketBall::Start_Shotclock(){
  shotclock_run=true;
  sc_last_millis=millis();
}

void PROscoreBasketBall::Stop_Shotclock(){
  shotclock_run=false;
}

bool PROscoreBasketBall::get_ShotclockRun(){
  return shotclock_run;
}

uint8_t PROscoreBasketBall::get_ShotlockSecond(){
  return shotclock_second;
}

uint8_t PROscoreBasketBall::get_ShotclockMillis(){
  return shotclock_millis;
}

String PROscoreBasketBall::getString_Shotclock(){
  char ch_Shotclock[4]="";

  if(shotclock_second<10){
    sprintf(ch_Shotclock,"0%d.%d", shotclock_second, shotclock_millis);
  }else{
    sprintf(ch_Shotclock,"%d.%d", shotclock_second, shotclock_millis);
  }

  return String(ch_Shotclock);
}

bool PROscoreBasketBall::get_SC_Critic(){
  if(shotclock_second<10){
    return true;
  }else{
    return false;
  }
}








