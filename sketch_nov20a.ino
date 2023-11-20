const int motorPin = 3;

void setup() {
  pinMode(motorPin, OUTPUT);
}

void loop() {
  int targetSpeed = 255;

  analogWrite(motorPin, targetSpeed);
}
