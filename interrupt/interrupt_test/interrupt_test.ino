const byte interruptPin = 2;  // input pin that the interruption will be attached to
volatile byte state = LOW;  // variable that will be updated in the ISR

void setup() {
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(4, OUTPUT); //we use High and Low for common cathod
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  pinMode(11, OUTPUT);
  pinMode(12, INPUT);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
}

void loop() {
  digitalWrite(4, state);
  digitalWrite(5, state);
  digitalWrite(6, state);
  digitalWrite(7, state);
  Serial.println(state);

  pinMode(12, OUTPUT);
  delay(1000);
  pinMode(12, INPUT);
  delay(1000);
  
}

void blink() {
  pinMode(11, OUTPUT);
  state = !state;
}