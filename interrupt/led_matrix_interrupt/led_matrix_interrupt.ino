unsigned long globalStartTime;
unsigned long startTime;
const byte interruptPin = 2;  // input pin that the interruption will be attached to
volatile byte state = LOW;  // variable that will be updated in the ISR
int selectedLayer = 0; // 0 or 1

void setup() {
  // interrupt part
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR, LOW);

  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(4, OUTPUT);  //we use High and Low for common cathod
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  digitalWrite(11, LOW);
  digitalWrite(12, LOW);

  globalStartTime = millis();
  startTime = millis();
}

int maximumValue = 255;
unsigned long totalTime = 5 * 1000;
float ledState = 0;
bool isIncreasing = true;

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - globalStartTime;  // unused
  unsigned long animationProgressTime = currentTime - startTime;

  float progressRatio = (float)animationProgressTime / totalTime;
  
  // ping pong effect
  if (progressRatio == 1.) {
    startTime = currentTime;
    isIncreasing = !isIncreasing; // switch to other state (decrease/increase)
  } 

  if (isIncreasing) {
    // increase - fade in
    ledState = ((floor((easeInSine(progressRatio)) * maximumValue)));
  } else {
    // decrease - fade out
    ledState = (floor((1. - (easeOutSine(progressRatio))) * maximumValue));
  }

  // Serial.println(progressRatio);

  // reset elapsed time
  if (animationProgressTime > totalTime) {
    startTime = currentTime;
  }


  analogWrite(5, ledState);
  analogWrite(6, ledState);
  analogWrite(9, ledState);
  analogWrite(10, ledState);

  if (state) {
    enableLayer(0);
  } else {
    disableLayer(0);
  }
  
  
}

void ISR() {
  state = !state;
}

// OUTPUT: when LOW, it is grounded, when HIGH it is powered
void enableLayer(int layer) {
  // Serial.print("enable");
  if (layer == 0) {
    pinMode(11, OUTPUT);
  }
  else if (layer == 1) {
    pinMode(12, OUTPUT);
  }
}

void disableLayer(int layer) {
  // Serial.print("disable");
  if (layer == 0) {
    pinMode(11, INPUT);
  }
  else if (layer == 1) {
    pinMode(12, INPUT);
  }
}

float easeInSine(float x) {
  return 1.0 - cos((x * PI) / 2.0);
}

float easeOutSine(float x) {
  return sin((x * PI) / 2.0);
}