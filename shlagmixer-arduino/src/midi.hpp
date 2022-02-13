#ifndef MIDI_H
#define MIDI_H
#include <Arduino.h>
#include <MIDIUSB.h>

void noteOn(byte channel, byte pitch, byte velocity);
void noteOff(byte channel, byte pitch, byte velocity);
void controlChange(byte channel, byte control, byte value);

#endif