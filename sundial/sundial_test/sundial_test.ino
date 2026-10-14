#include "pin_mapping.h"

int mux1Readings[NUM_CHANNELS];
int mux2Readings[NUM_CHANNELS];

void setup() {
  Serial.begin(115200);

  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);

  pinMode(MUX1_OUTPUT, INPUT);
  pinMode(MUX2_OUTPUT, INPUT);

  digitalWrite(S0_PIN, LOW);
  digitalWrite(S1_PIN, LOW);
  digitalWrite(S2_PIN, LOW);

  analogReadResolution(12);

  // Safe initial values for unused channels
  for (int channel = 0; channel < NUM_CHANNELS; channel++) {
    mux1Readings[channel] = ADC_MAX;
    mux2Readings[channel] = ADC_MAX;
  }

  Serial.println();
  Serial.println("12-hour light sundial started");

  printChannelMappings();
}

void loop() {
  readActiveSensors();

  int darkestMux = -1;
  int darkestChannel = -1;
  int lowestReading = ADC_MAX + 1;

  findDarkestSensor(
    darkestMux,
    darkestChannel,
    lowestReading
  );

  if (darkestMux == -1 || darkestChannel == -1) {
    Serial.println("No active sensors configured.");
    delay(500);
    return;
  }

  printActiveReadings();

  int detectedHour;
  int darkestRaw;

  if (darkestMux == 1) {
    detectedHour = HOUR_FOR_MUX1[darkestChannel];
    darkestRaw = mux1Readings[darkestChannel];
  } else {
    detectedHour = HOUR_FOR_MUX2[darkestChannel];
    darkestRaw = mux2Readings[darkestChannel];
  }

  int darkness = ADC_MAX - darkestRaw;

  Serial.print("SUNDIAL HOUR: ");
  printHour(detectedHour);

  Serial.print(" | MUX");
  Serial.print(darkestMux);

  Serial.print(" X");
  Serial.print(darkestChannel);

  Serial.print(" | raw=");
  Serial.print(darkestRaw);

  Serial.print(" | darkness=");
  Serial.println(darkness);

  Serial.println("----------------------------------------");

  delay(200);
}

// --------------------------------------------------
// Read active channels on both multiplexers
// --------------------------------------------------

void readActiveSensors() {
  for (int channel = 0; channel < NUM_CHANNELS; channel++) {
    bool mux1Active = HOUR_FOR_MUX1[channel] != 0;
    bool mux2Active = HOUR_FOR_MUX2[channel] != 0;

    if (!mux1Active && !mux2Active) {
      mux1Readings[channel] = ADC_MAX;
      mux2Readings[channel] = ADC_MAX;
      continue;
    }

    // Both multiplexers share these selector pins
    selectMuxChannel(channel);

    if (mux1Active) {
      mux1Readings[channel] = readAveraged(MUX1_OUTPUT);
    } else {
      mux1Readings[channel] = ADC_MAX; // set the reading as ADC_MAX to prevent it from floating result
    }

    if (mux2Active) {
      mux2Readings[channel] = readAveraged(MUX2_OUTPUT);
    } else {
      mux2Readings[channel] = ADC_MAX;
    }
  }
}

// --------------------------------------------------
// Select X0–X7 on both CD4051 multiplexers
// --------------------------------------------------

void selectMuxChannel(int channel) {
  digitalWrite(S0_PIN, bitRead(channel, 0));
  digitalWrite(S1_PIN, bitRead(channel, 1));
  digitalWrite(S2_PIN, bitRead(channel, 2));

  delay(1);
}

// --------------------------------------------------
// Read and average one selected mux output
// --------------------------------------------------

int readAveraged(int analogPin) {
  long total = 0;

  // Discard first ADC reading after switching
  analogRead(analogPin);
  delayMicroseconds(100);

  for (int sample = 0; sample < NUM_SAMPLES; sample++) {
    total += analogRead(analogPin);
    delayMicroseconds(100);
  }

  return total / NUM_SAMPLES;
}

// --------------------------------------------------
// Find darkest sensor across MUX1 and MUX2
// --------------------------------------------------

void findDarkestSensor(
  int &darkestMux,
  int &darkestChannel,
  int &lowestReading
) {
  darkestMux = -1;
  darkestChannel = -1;
  lowestReading = ADC_MAX + 1;

  for (int channel = 0; channel < NUM_CHANNELS; channel++) {
    // Check MUX1
    if (
      HOUR_FOR_MUX1[channel] != 0 &&
      mux1Readings[channel] < lowestReading
    ) {
      lowestReading = mux1Readings[channel];
      darkestMux = 1;
      darkestChannel = channel;
    }

    // Check MUX2
    if (
      HOUR_FOR_MUX2[channel] != 0 &&
      mux2Readings[channel] < lowestReading
    ) {
      lowestReading = mux2Readings[channel];
      darkestMux = 2;
      darkestChannel = channel;
    }
  }
}

// --------------------------------------------------
// Print active readings from both multiplexers
// --------------------------------------------------

void printActiveReadings() {
  bool firstReading = true;

  // read from mux 1 and mux 2
  // for each sensor read the channel
  // the channel that is unused is marked as 0 in pin_mapping.h and skipped in this reading
  for (int channel = 0; channel < NUM_CHANNELS; channel++) {
    if (HOUR_FOR_MUX1[channel] != 0) {
      if (!firstReading) {
        Serial.print(" | ");
      }

      printOneReading(
        1,
        channel,
        HOUR_FOR_MUX1[channel],
        mux1Readings[channel]
      );

      firstReading = false;
    }

    if (HOUR_FOR_MUX2[channel] != 0) {
      if (!firstReading) {
        Serial.print(" | ");
      }

      printOneReading(
        2,
        channel,
        HOUR_FOR_MUX2[channel],
        mux2Readings[channel]
      );

      firstReading = false;
    }
  }

  Serial.println();
}

// --------------------------------------------------
// Print one sensor
// --------------------------------------------------
// the brighter -> the higher the reading
//  the darker -> the lower the reading
void printOneReading(
  int muxNumber,
  int channel,
  int hour,
  int raw
) {
  int darkness = ADC_MAX - raw;

  Serial.print("M");
  Serial.print(muxNumber);

  Serial.print(":X");
  Serial.print(channel);

  Serial.print("[");
  printHour(hour);
  Serial.print("]");

  Serial.print(": raw=");
  Serial.print(raw);

  Serial.print(", dark=");
  Serial.print(darkness);
}

// --------------------------------------------------
// Print both mapping tables once at startup
// --------------------------------------------------

void printChannelMappings() {
  Serial.println("Active channel mappings:");

  for (int channel = 0; channel < NUM_CHANNELS; channel++) {
    Serial.print("MUX1 X");
    Serial.print(channel);
    Serial.print(" -> ");

    if (HOUR_FOR_MUX1[channel] == 0) {
      Serial.print("unused");
    } else {
      printHour(HOUR_FOR_MUX1[channel]);
    }

    Serial.print(" | MUX2 X");
    Serial.print(channel);
    Serial.print(" -> ");

    if (HOUR_FOR_MUX2[channel] == 0) {
      Serial.println("unused");
    } else {
      printHour(HOUR_FOR_MUX2[channel]);
      Serial.println();
    }
  }

  Serial.println("----------------------------------------");
}

void printHour(int hour) {
  Serial.print(hour);
  Serial.print(":00");
}