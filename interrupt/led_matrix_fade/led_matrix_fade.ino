unsigned long globalStartTime;
unsigned long startTime;

void setup() {
  // put your setup code here, to run once:
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
unsigned long totalTime = 10 * 1000;
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
    ledState = (floor(easeInSine(progressRatio) * maximumValue));
  } else {
    // decrease - fade out
    ledState = (floor((1. - (easeOutSine(progressRatio))) * maximumValue));
  }

  Serial.println(progressRatio);

  // reset elapsed time
  if (animationProgressTime > totalTime) {
    startTime = currentTime;
  }


  analogWrite(5, ledState);
  analogWrite(6, ledState);
  analogWrite(9, ledState);
  analogWrite(10, ledState);
  
}

float easeInSine(float x) {
  return 1.0 - cos((x * PI) / 2.0);
}

float easeOutSine(float x) {
  return sin((x * PI) / 2.0);
}