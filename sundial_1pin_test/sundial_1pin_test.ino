// -------- Pick what you want to test --------
const int MUX_OUTPUT_PIN = A1;  // Change to A1 for the second MUX
const int MUX_CHANNEL = 0;      // Y0 = channel 0
// --------------------------------------------

// Shared CD4051 select pins
const int S0_PIN = 2;
const int S1_PIN = 3;
const int S2_PIN = 4;

void setup() {
  Serial.begin(115200);

  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);

  pinMode(MUX_OUTPUT_PIN, INPUT);

  // Nano 33 IoT: use 12-bit ADC readings (0–4095)
  analogReadResolution(12);

  selectMuxChannel(MUX_CHANNEL);

  Serial.println("Single MUX channel test");
  Serial.print("Analog output pin: A");
  Serial.println(MUX_OUTPUT_PIN - A0);
  Serial.print("MUX channel: Y");
  Serial.println(MUX_CHANNEL);
}

void loop() {
  selectMuxChannel(MUX_CHANNEL);

  // Discard the first reading after selecting the channel
  analogRead(MUX_OUTPUT_PIN);
  delayMicroseconds(100);

  int sensorValue = analogRead(MUX_OUTPUT_PIN);

  Serial.print("Y");
  Serial.print(MUX_CHANNEL);
  Serial.print(" = ");
  Serial.println(sensorValue);

  delay(250);
}

void selectMuxChannel(int channel) {
  digitalWrite(S0_PIN, bitRead(channel, 0));
  digitalWrite(S1_PIN, bitRead(channel, 1));
  digitalWrite(S2_PIN, bitRead(channel, 2));

  delayMicroseconds(100);
}