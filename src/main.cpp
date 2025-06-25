#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

const int butt1 = 10;
const int butt2 = 2;
const int buzz1 = 11;

int mainState = 0;
int timerMinutes = 1;
int breakMinutes = 1;
int minute = 60;

unsigned long prevMills = 0;
unsigned long timeInt = 1000;

void timerMode() {

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 30);
  display.println("TimerMode");
  display.display();
}

void pomoMode() {

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 30);
  display.println("PomoMode");
  display.display();
}

void pomoConfig(int minutes) {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.println("Select Work Time:");
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(50, 30);
  display.println(minutes);
  display.display();
}

void breakTime(int minutes) {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.println("Select Break Time");
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(50, 30);
  display.println(minutes);
  display.display();
}

void timerConfig(int minutes) {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.println("Select Minutes:");
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(50, 30);
  display.println(minutes);
  display.display();
}

void runTimer() {

  unsigned long cMills = millis();

  if (cMills - prevMills >= timeInt) {
    prevMills = cMills;
    minute--;

    if ((minute < 0 && timerMinutes > 0) || minute == 60) {

      minute = 59;
      timerMinutes--;
    } else if (minute < 0 && timerMinutes == 0) {
      Serial.println("TIMER DONE!");
      mainState = 7;
    }
  }

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(20, 30);
  display.print(timerMinutes);
  display.print(":");
  display.println(minute);
  display.display();
}

void runWorkTimer() {

  unsigned long cMills = millis();

  if (cMills - prevMills >= timeInt) {
    prevMills = cMills;
    minute--;

    if ((minute < 0 && timerMinutes > 0) || minute == 60) {

      minute = 59;
      timerMinutes--;
    } else if (minute < 0 && timerMinutes == 0) {
      Serial.println("TIMER DONE!");
      mainState = 10;
    }
  }

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(20, 30);
  display.print(timerMinutes);
  display.print(":");
  display.println(minute);
  display.display();
}

void runBreakTimer() {

  unsigned long cMills = millis();

  if (cMills - prevMills >= timeInt) {
    prevMills = cMills;
    minute--;

    if ((minute < 0 && breakMinutes> 0) || minute == 60) {

      minute = 59;
      breakMinutes--;
    } else if (minute < 0 && breakMinutes == 0) {
      Serial.println("TIMER DONE!");
      mainState = 7;
    }
  }

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(20, 30);
  display.print(timerMinutes);
  display.print(":");
  display.println(minute);
  display.display();
}
void buzzerTone() {

  Serial.println("BUZZER ON");
  digitalWrite(buzz1, HIGH);
  delay(200);
  digitalWrite(buzz1, LOW);
  delay(200);
  digitalWrite(buzz1, HIGH);
  delay(200);
  digitalWrite(buzz1, LOW);
  delay(200);
  digitalWrite(buzz1, HIGH);
  delay(200);
  digitalWrite(buzz1, LOW);
}

void timerDone() {

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 30);
  display.println("TIME'S UP");
  display.display();
  buzzerTone();
}
void setup() {

  pinMode(butt1, INPUT_PULLUP);
  pinMode(butt2, INPUT_PULLUP);
  pinMode(buzz1, OUTPUT);

  Serial.begin(9600);
  display.begin(0x3C); // initialize with the I2C addr 0x3D (for the 128x64)

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.println("Pomoduino");
  display.display();
  delay(2000);
}

void loop() {
  int but1Val = digitalRead(butt1);
  int but2Val = digitalRead(butt2);
  delay(200);

  if ((but1Val == 0 && mainState == 1) || (but1Val == 0 && mainState == 2)) {
    mainState++;
    Serial.println(mainState);

  } else if (but2Val == 0 && mainState == 1) {
    Serial.println("Timer Config");
    mainState = 4;

  } else if (but2Val == 0 && mainState == 2) {
    Serial.println("Pomo Config");
    mainState = 5;

  } else if (but2Val == 0 && mainState == 4) {
    if (timerMinutes > 59) {
      timerMinutes = 1;
    } else {
      timerMinutes++;
    }

  } else if (but2Val == 0 && mainState == 5) {
    if (timerMinutes > 59) {
      timerMinutes = 1;
    } else {
      timerMinutes++;
    }

  } else if (but1Val == 0 && mainState == 5) {

    timerMinutes = (timerMinutes > 0) ? timerMinutes - 1 : timerMinutes;
    mainState = 8;

  } else if (but2Val == 0 && mainState == 8) {
    if (breakMinutes > 59) {
      breakMinutes = 1;
    } else {
      breakMinutes++;
    }
  } else if (but1Val == 0 && mainState == 8) {

    breakMinutes = (breakMinutes > 0) ? breakMinutes - 1 : breakMinutes;
    mainState = 9;

  } else if (but1Val == 0 && mainState == 7) {
    minute = 60;
    mainState = 1;
  } else if (but1Val == 0 && mainState == 4) {

    timerMinutes = (timerMinutes > 0) ? timerMinutes - 1 : timerMinutes;
    mainState = 8;

  } else {
    Serial.println(mainState);

    switch (mainState) {
    case 1:
      Serial.println("mode1");
      timerMode();
      break;
    case 2:
      Serial.println("mode2");
      pomoMode();
      break;
    case 4:
      timerConfig(timerMinutes);
      break;
    case 5:
      pomoConfig(timerMinutes);
      break;
    case 6:
      runTimer();
      break;
    case 7:
      timerDone();
      break;
    case 8:
      breakTime(breakMinutes);
      break;
    case 9:
      runWorkTimer();
      break;
    case 10:
      runBreakTimer();
      break;
    default:
      Serial.println("Reset State");
      mainState = 1;
      break;
    }
  }
}
