/*
    PROscore Basketball Library

    by: KennyNeutron
    Date Started: 10-17-2023
    Date Finished: N/A
*/
#ifndef PROSCOREBASKETBALL_H
#define PROSCOREBASKETBALL_H

#include "Arduino.h"

class PROscoreBasketBall {
public:
  PROscoreBasketBall();
  void begin();

  void loop();

  void Start_GameTime();
  void Stop_GameTime();
  bool get_GameTimeRun();

  uint8_t get_GameTime_Minute();
  uint8_t get_GameTime_Second();
  uint8_t get_GameTime_Millis();
  String getString_GameTime();

  void Start_Shotclock();
  void Stop_Shotclock();
  bool get_ShotclockRun();
  bool get_SC_Critic();

  uint8_t get_ShotlockSecond();
  uint8_t get_ShotclockMillis();
  String getString_Shotclock();

  void inc_HomeScore();
  void dec_HomeScore();
  void inc_GuestScore();
  void dec_GuestScore();

  void inc_HomeFoul();
  void dec_HomeFoul();
  void inc_GuestFoul();
  void dec_GuestFoul();

  void inc_HomeTOut();
  void dec_HomeTOut();
  void inc_GuestTOut();
  void dec_GuestTOut();

  void inc_Period();
  void dec_Period();

  void SC_Reset24();
  void SC_Reset14();
  void SC_Reset12();

  void Buzzer(bool BuzzerStatus);

  void BallPos();


private:
  //Interrupt
  unsigned int ISR_TMR_GT=0;
  unsigned int ISR_TMR_SC=0;


  //PROscore Vars
  uint8_t gametime_minute = 10;
  uint8_t gametime_second = 0;
  uint8_t gametime_millis = 0;
  bool gametime_run=false;

  uint8_t shotclock_second = 24;
  uint8_t shotclock_millis = 0;
  bool shotclock_run=false;

  uint8_t homescore = 0;
  uint8_t guestscore = 0;

  uint8_t homefoul = 0;
  uint8_t guestfoul = 0;

  uint8_t homeTOut = 0;
  uint8_t guestTOut = 0;

  uint8_t period = 1;

  bool buzzer_enable = false;

  bool ballpos_enable = false;
  bool ballpos = false;


  uint32_t gt_last_millis=0;
  uint32_t sc_last_millis=0;


};

#endif