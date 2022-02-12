#include "MIDIUSB.h"

#define FADERS_COUNT 5
#define SWITCHES_COUNT 10

const int FADERS_PINS[] = { A0, A1, A2, A3, A4 };
const int SWITCHES_PINS[] = { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

int SWITCHES_STATES[SWITCHES_COUNT];
int FADERS_STATES[FADERS_COUNT];

void setup() {
  for (int i = 0; i < FADERS_COUNT; i++) {
    pinMode(FADERS_PINS[i], INPUT);
  }
  for (int i = 0; i < SWITCHES_COUNT; i++) {
    pinMode(SWITCHES_PINS[i], INPUT_PULLUP);

    int readValue = digitalRead(SWITCHES_PINS[i]);
    SWITCHES_STATES[i] = readValue;
    if (readValue == HIGH) {
      noteOn(0, 12 + i, 64);
    }
  }

  MidiUSB.flush();
}

void loop() {
  for (int i = 0; i < FADERS_COUNT; i++) {
    int readState = analogRead(FADERS_PINS[i]);
    if (
          readState < FADERS_STATES[i] - 1
      ||  readState > FADERS_STATES[i] + 1
    ) {
      FADERS_STATES[i] = readState;

      controlChange(0, 32 + i, map(readState, 0, 1023, 0, 127));
      MidiUSB.flush();
    }
  }

  for (int i = 0; i < SWITCHES_COUNT; i++) {
    int readState = digitalRead(SWITCHES_PINS[i]);
    if (readState != SWITCHES_STATES[i]) {      
      SWITCHES_STATES[i] = readState;
      if (readState == HIGH) {
        noteOn(0, 12 + i, 64);
      } else {
        noteOff(0, 12 + i, 64);
      }

      MidiUSB.flush();
    }
  }

  delay(25);
}



// MIDIUSB stuff
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}