#include <LiquidCrystal.h>
#include <TimerOne.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

volatile unsigned int timer = 0;
const unsigned int lessonDuration = 45;
const unsigned int breakDuration = 10;
volatile byte row = 0;
const byte firstRow = 0;
const byte secondRow = 1;
const byte buttonLessonPin = 2;
const byte buttonBreakPin = 3;
volatile boolean timerStarted = false;
const byte alarmDuration = 5;
const byte jinglePin = 13;

void initializeTimer() {
  Timer1.initialize();
  Timer1.attachInterrupt(tick);
  Timer1.stop();
}

void initializeDisplay() {
  lcd.begin(16, 2);
  printTimer(lessonDuration, firstRow);
  printTimer(breakDuration, secondRow);
}

void initializeButtons() {
  attachInterrupt(digitalPinToInterrupt(buttonBreakPin), startBreakTimer, HIGH);
  attachInterrupt(digitalPinToInterrupt(buttonLessonPin), startLessonTimer, HIGH);
}

void initializeJingle() {
  pinMode(jinglePin, OUTPUT);
}

void setup() {
  initializeTimer();
  initializeDisplay();
  initializeButtons();
  initializeJingle();
}

void tick() {
  if (timer > 0) {
    timer -= 1;
    if (timer == 0) {
      Timer1.stop();
    }
  }
}

void printTimer(unsigned int t, byte r) {
  lcd.setCursor(5, r);
  lcd.print(timerToString(t));
}

char* timerToString(unsigned int t) {
  char res[6];
  sprintf(res, "%02u:%02u", timer / 60, t % 60);
  return res;
}

void startBreakTimer() {
  startTimer(breakDuration + 1, secondRow);
}

void startLessonTimer() {
  startTimer(lessonDuration + 1, firstRow);
}

void startTimer(unsigned int duration, byte r) {
  if (!timerStarted) {
    timer = duration;
    row = r;
    timerStarted = true;
    lcd.clear();
    Timer1.start();
  }
}

void alarm() {
  for (byte i = 0; i < alarmDuration; i++) {
    printTimer(timer, row);
    tone(jinglePin, 1000);
    delay(500);
    lcd.clear();
    noTone(jinglePin);
    delay(500);
  }
  timerStarted = false;
  initializeDisplay();
}

void loop() {
  if (timerStarted) {
    printTimer(timer, row);
    if (timer == 0) {
      alarm();
    }
  }
  delay(100);
}

