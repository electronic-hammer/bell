const byte buttonLessonPin = 2;
const byte buttonBreakPin = 3;
const byte jinglePin = 13;
// in ms
const unsigned long lessonDuration = 10000;
const unsigned long breakDuration = 5000;
volatile long timer = 0;
const unsigned int tick = 1000;
const unsigned int jingleDuration = 1000;

void setup() {
  pinMode(buttonLessonPin, INPUT);
  pinMode(buttonBreakPin, INPUT);
  pinMode(jinglePin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(buttonLessonPin), startLessonTimer, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonBreakPin), startBreakTimer, CHANGE);
}

void loop() {
  if (timer > 0) {
    timer -= tick;
    if (timer <= 0) {
      tone(jinglePin, 1000, jingleDuration);
    }
  }
  delay(tick);
}

void startLessonTimer() {
  timer = lessonDuration;
}

void startBreakTimer() {
  timer = breakDuration;
}

