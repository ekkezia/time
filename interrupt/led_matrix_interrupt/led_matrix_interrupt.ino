const byte ledPin = 4;
const byte interruptPin = 2;  // input pin that the interruption will be attached to
volatile byte state = LOW;  // variable that will be updated in the ISR
int selectedLayer = 0; // 0 or 1

void setup() {
  // pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, LOW);

  digitalWrite(11, LOW);  //we change pinMode on cathod / GNDto control current
  digitalWrite(12, LOW);

  pinMode(4, OUTPUT); //we use High and Low for common cathod
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {
  // digitalWrite(ledPin, state);
  // digitalWrite(4, HIGH);
  // digitalWrite(5, HIGH);
  // digitalWrite(6, HIGH);
  // digitalWrite(7, HIGH);
  // enableLayer(0);
  // disableLayer(1);
  // delay(500);
  // enableLayer(1);
  // disableLayer(0);
  // delay(500);
}

void blink() {
  state = (selectedLayer + 1) % 2;
  
  enableLayer(state);
  disableLayer(1 - state);
}


// OUTPUT: when LOW, it is grounded, when HIGH it is powered
void enableLayer(int layer) {
  Serial.print("enable");
  Serial.println(layer);
  if (layer == 0) {
    pinMode(11, OUTPUT);
  }
  else if (layer == 1) {
    pinMode(12, OUTPUT);
  }
}

void disableLayer(int layer) {
  Serial.print("disable");
  Serial.println(layer);
  if (layer == 0) {
    pinMode(11, INPUT);
  }
  else if (layer == 1) {
    pinMode(12, INPUT);
  }
}