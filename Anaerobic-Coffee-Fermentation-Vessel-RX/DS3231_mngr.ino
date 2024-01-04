void DS3231_setTime(uint8_t bHour, uint8_t bMinute, uint8_t bSecond, uint8_t bMonth, uint8_t bDate, uint8_t bYear) {
  //SET DATE & TIME
  myRTC.setClockMode(false);  // TRUE set to 12h  || FALSE set to 24h
  myRTC.setHour(bHour);
  myRTC.setMinute(bMinute);
  myRTC.setSecond(bSecond);
  myRTC.setDate(bDate);
  myRTC.setMonth(bMonth);
  myRTC.setYear(bYear);
}

String DS3231_getTimeString() {
  String str_Hour = "";
  String str_Minute = "";
  String str_Second = "";
  String str_TimeString = "";


  if (myRTC.getHour(h12Flag, pmFlag) < 10) {
    str_Hour = "0" + String(myRTC.getHour(h12Flag, pmFlag));
  } else {
    str_Hour = String(myRTC.getHour(h12Flag, pmFlag));
  }

  if (myRTC.getMinute() < 10) {
    str_Minute = "0" + String(myRTC.getMinute());
  } else {
    str_Minute = String(myRTC.getMinute());
  }

  if (myRTC.getSecond() < 10) {
    str_Second = "0" + String(myRTC.getSecond());
  } else {
    str_Second = String(myRTC.getSecond());
  }

  str_TimeString = str_Hour + ":" + str_Minute + ":" + str_Second;

  return str_TimeString;
}

String DS3231_getDateString() {
  String str_Month = "";
  String str_Date = "";
  String str_Year = "";
  String str_DateString = "";

  if (myRTC.getMonth(century) < 10) {
    str_Month = "0" + String(myRTC.getMonth(century), DEC);
  } else {
    str_Month = String(myRTC.getMonth(century), DEC);
  }

  if (myRTC.getDate() < 10) {
    str_Date = "0" + String(myRTC.getDate(), DEC);
  } else {
    str_Date = String(myRTC.getDate(), DEC);
  }

  if (myRTC.getYear() < 10) {
    str_Year = "200" + String(myRTC.getYear(), DEC);
  } else {
    str_Year = "20" + String(myRTC.getYear(), DEC);
  }

  str_DateString = str_Month + "/" + str_Date + "/" + str_Year;

  return str_DateString;
}