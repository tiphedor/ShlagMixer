#include "midi.hpp"

#define FADERS_COUNT 5
#define SWITCHES_COUNT 10
const int FADERS_PINS[] = { A0, A1, A2, A3, A4 };
const int SWITCHES_PINS[] = { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
#define DEBOUNCE_THR 2

int SWITCHES_STATES[SWITCHES_COUNT];
int FADERS_STATES[FADERS_COUNT];

void setup() {
  // Faders
  for (int i = 0; i < FADERS_COUNT; i++) {
    pinMode(FADERS_PINS[i], INPUT);
    FADERS_STATES[i] = analogRead(FADERS_PINS[i]);
  }


  // Switches
  for (int i = 0; i < SWITCHES_COUNT; i++) {
    pinMode(SWITCHES_PINS[i], INPUT_PULLUP);
    SWITCHES_STATES[i] = digitalRead(SWITCHES_PINS[i]);
  }
}

void loop() {
  // Faders
  // We send data on channel 32, 33, 34, 35, ... etc, this is arbitrary and can be changed as long as its also changed in the ahk file.
  for (int i = 0; i < FADERS_COUNT; i++) {
    int readState = analogRead(FADERS_PINS[i]);

    // If currentState - DEBOUNCE_THR < newState < currentStatet + DEBOUNCE_THR, threshold is used because the faders are quite noisy
    if (readState < FADERS_STATES[i] - DEBOUNCE_THR ||  readState > FADERS_STATES[i] + DEBOUNCE_THR) {
      FADERS_STATES[i] = readState; // update the current state

      // Send the note, we read a value withn 0-1023, MIDI uses 0-127, so remap the value
      controlChange(0, 32 + i, map(readState, 0, 1023, 0, 127));
    }
  }

  // Toggle switches
  // We send notes 12, 13, 14, ... etc, this is arbitrary and can be changed as long as its also changed in the ahk file.
  for (int i = 0; i < SWITCHES_COUNT; i++) {
    int readState = digitalRead(SWITCHES_PINS[i]);

    // Has button state changed ?
    if (readState != SWITCHES_STATES[i]) {      
      SWITCHES_STATES[i] = readState;

      if (readState == HIGH) {
        noteOn(0, 12 + i, 64);
      } else {
        noteOff(0, 12 + i, 64);
      }
    }
  }

  midiEventPacket_t rx;
  // Read incoming data - when we receive note 11 - send the current state.
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      byte command = rx.byte1;
      byte note = rx.byte2;

      // 144 = noteOn command - form the MIDI standard
      // 11 = arbitrary and can be changed as long as its also changed in the ahk file.
      // We don't care about velocity
      if (command == 144 && note == 11) {
        for (int i = 0; i < FADERS_COUNT; i++) {
          controlChange(0, 32 + i, map(FADERS_STATES[i], 0, 1023, 0, 127));
        }

        for (int i = 0; i < SWITCHES_COUNT; i++) {
          if (SWITCHES_STATES[i] == HIGH) {
            noteOn(0, 12 + i, 64);
          } else {
            noteOff(0, 12 + i, 64);
          }
        }
      }
    }
  } while (rx.header != 0);
  

  MidiUSB.flush();
  delay(25);
}
