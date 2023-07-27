#define BLYNK_DEBUG

#define BLYNK_TEMPLATE_ID           "TMPL4YRuYj5-u"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "Ug4sODAWWFoCFGf-4OncoqUuysstfPpV"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <OneButton.h>

char ssid[] = "KBL-Home 2.4";
char pass[] = "kblhome21";

BlynkTimer timer;

struct ServoControl {
  int timer;
  int servoPosition;
  int direction;
  uint8_t servoPin;
  uint8_t pwmPin;
  uint8_t pwmValue;
  OneButton button;
};

const uint8_t MAX_DEG = 180;
const uint8_t STEP = MAX_DEG / 10;
const unsigned long LONG_PRESS_INTERVAL = 1000;

const uint8_t SERVO_1_PIN = D8;
const uint8_t SERVO_2_PIN = D7;
const uint8_t SERVO_3_PIN = D6;

const uint8_t BUTTON_1_PIN = D4;
const uint8_t BUTTON_2_PIN = D3;
const uint8_t BUTTON_3_PIN = D2;

Servo servo1;
Servo servo2;
Servo servo3;

OneButton button1 = OneButton(
  BUTTON_1_PIN,
  true,
  false);

OneButton button2 = OneButton(
  BUTTON_2_PIN,
  true,
  false);

OneButton button3 = OneButton(
  BUTTON_3_PIN,
  true,
  false);

ServoControl servoControls[3] = {
  { 0, MAX_DEG, -1, SERVO_1_PIN, D5, 255, button1 },
  { 0, MAX_DEG, -1, SERVO_2_PIN, D0, 255, button2 },
  { 0, MAX_DEG, -1, SERVO_3_PIN, D1, 255, button3 }
};

void setup() {
  Serial.begin(115200);

  servo1.attach(SERVO_1_PIN);
  servo2.attach(SERVO_2_PIN);
  servo3.attach(SERVO_3_PIN);

  controlServo(0, servoControls[0].servoPosition);
  controlServo(1, servoControls[1].servoPosition);
  controlServo(2, servoControls[2].servoPosition);

  analogWrite(servoControls[0].pwmPin, servoControls[0].pwmValue);
  analogWrite(servoControls[1].pwmPin, servoControls[1].pwmValue);
  analogWrite(servoControls[2].pwmPin, servoControls[2].pwmValue);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);
  pinMode(BUTTON_3_PIN, INPUT);

  button1.attachClick(button1Clicked);
  button2.attachClick(button2Clicked);
  button3.attachClick(button3Clicked);

  button1.attachLongPressStart(button1LongPressStart);
  button2.attachLongPressStart(button2LongPressStart);
  button3.attachLongPressStart(button3LongPressStart);

  button1.attachLongPressStop(button1LongPressStop);
  button2.attachLongPressStop(button2LongPressStop);
  button3.attachLongPressStop(button3LongPressStop);

  button1.attachDuringLongPress(button1DuringLongPress);
  button2.attachDuringLongPress(button2DuringLongPress);
  button3.attachDuringLongPress(button3DuringLongPress);
  Serial.println(button1.state());
}

void loop() {
  Blynk.run();
  button1.tick();
  button2.tick();
  button3.tick();
}

BLYNK_WRITE(V4) {
  int sliderPosition = param.asInt();
  servoControls[0].servoPosition = sliderPosition;
  servoControls[0].pwmValue = map(sliderPosition, 0, 180, 0, 255);
  controlServo(0, servoControls[0].servoPosition);
  analogWrite(servoControls[0].pwmPin, servoControls[0].pwmValue);
}

BLYNK_WRITE(V5) {
  int sliderPosition = param.asInt();
  servoControls[1].servoPosition = sliderPosition;
  servoControls[1].pwmValue = map(sliderPosition, 0, 180, 0, 255);
  controlServo(1, servoControls[1].servoPosition);
  analogWrite(servoControls[1].pwmPin, servoControls[1].pwmValue);
}

BLYNK_WRITE(V6) {
  int sliderPosition = param.asInt();
  servoControls[2].servoPosition = sliderPosition;
  servoControls[2].pwmValue = map(sliderPosition, 0, 180, 0, 255);
  controlServo(2, servoControls[2].servoPosition);
  analogWrite(servoControls[2].pwmPin, servoControls[2].pwmValue);
}

void button1Clicked() {
  buttonClicked(0);
}

void button2Clicked() {
  buttonClicked(1);
}

void button3Clicked() {
  buttonClicked(2);
}

void button1LongPressStart() {
  buttonLongPressStart(0);
}

void button2LongPressStart() {
  buttonLongPressStart(1);
}

void button3LongPressStart() {
  buttonLongPressStart(2);
}

void button1LongPressStop() {
  buttonLongPressStop(0);
}

void button2LongPressStop() {
  buttonLongPressStop(1);
}

void button3LongPressStop() {
  buttonLongPressStop(2);
}

void button1DuringLongPress() {
  buttonDuringLongPress(0);
}

void button2DuringLongPress() {
  buttonDuringLongPress(1);
}

void button3DuringLongPress() {
  buttonDuringLongPress(2);
}

void buttonClicked(int index) {
  Serial.print("short click on button: ");
  Serial.print(index);
  Serial.print(" , position: ");
  ServoControl servoControl = servoControls[index];
  Serial.println(servoControl.servoPosition);
  if (servoControl.servoPosition > 0) {
    servoControl.servoPosition = 0;
  } else {
    servoControl.servoPosition = MAX_DEG;
  }
  servoControl.pwmValue = map(servoControl.servoPosition, 0, 180, 0, 255);
  controlServo(index, servoControl.servoPosition);
  analogWrite(servoControl.pwmPin, servoControl.pwmValue);
  servoControls[index] = servoControl;
}

void buttonLongPressStart(int index) {
  Serial.println("long click start");
  ServoControl servoControl = servoControls[index];
  servoControl.timer = 0;
  if (servoControl.servoPosition == 0) {
    servoControl.direction = 1;
  }
  if (servoControl.servoPosition == MAX_DEG) {
    servoControl.direction = -1;
  }
  servoControls[index] = servoControl;
}

void buttonLongPressStop(int index) {
  Serial.println("long click stop");
  servoControls[index].direction *= -1;
}

void buttonDuringLongPress(int index) {
  unsigned long currentMillis = millis();
  ServoControl servoControl = servoControls[index];
  if (currentMillis - servoControl.timer >= LONG_PRESS_INTERVAL) {
    servoControl.timer = currentMillis;
    if (servoControl.direction == 1 && servoControl.servoPosition < MAX_DEG) {
      servoControl.servoPosition += STEP;
    }
    if (servoControl.direction == -1 && servoControl.servoPosition > 0) {
      servoControl.servoPosition -= STEP;
    }
    servoControl.pwmValue = map(servoControl.servoPosition, 0, 180, 0, 255);
    controlServo(index, servoControl.servoPosition);
    analogWrite(servoControl.pwmPin, servoControl.pwmValue);
    Serial.print("servo1Position = ");
    Serial.println(servoControl.servoPosition);
  }
  servoControls[index] = servoControl;
}

void controlServo(int index, int servoPosition) {
  switch (index) {
    case 0:
      servo1.write(servoPosition);
      Blynk.virtualWrite(V4, servoPosition);
      break;
    case 1:
      servo2.write(servoPosition);
      Blynk.virtualWrite(V5, servoPosition);
      break;
    case 2:
      uint8_t mappedServoPosition = map(servoPosition, 0, MAX_DEG, 0, 130);
      Blynk.virtualWrite(V1, mappedServoPosition);
      servo3.write(mappedServoPosition);
      Blynk.virtualWrite(V6, servoPosition);
      break;
  }
}

/*
Add logic to prevent closing all 3 vents
*/