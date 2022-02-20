targetMidiDeviceName := "Arduino Leonardo"
global noteToStrip := { 21: 4, 20: 4, 19: 3, 18: 3, 17: 2, 16: 2, 15: 1, 14: 1, 13: 0, 12: 0}
global faderToStrip := { 36: 0, 35: 1, 34: 2, 33: 3, 32: 4 }

#include AutoHotKey-MIDI/Midi.ahk
#SingleInstance Force
#persistent
#NoEnv
SendMode Input

OnExit("DestroyRemote")
LoadRemote()

map(x,in_min,in_max,out_min,out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
}
HandleNote(event, statusToWrite) {
	note := event.noteNumber
	target := Mod(note, 2) == 0 ? "A1" : "B2"

	command := "Strip[" . noteToStrip[note] . "]." . target
	DllCall("VoicemeeterRemote64\VBVMR_SetParameterFloat", "AStr", command, "Int", statusToWrite)
}

midi := new Midi()
midiOpenRetCode := midi.OpenMidiInByName(targetMidiDeviceName)
if (midiOpenRetCode < 0) {
	MsgBox, Could not open MIDI device %targetMidiDeviceName%
}

LoadRemote() {
	VBVMRDLL := DllCall("LoadLibrary", "str", "C:\Program Files (x86)\VB\Voicemeeter\VoicemeeterRemote64.dll")
	retCode := DllCall("VoicemeeterRemote64\VBVMR_Login")
	
	if (%retCode% != 0 or %ErrorLevel% != 0) {	
		MsgBox, 0x10, Error, An error occured.
	}
}

DestroyRemote() {
	logoutRetCode := DllCall("VoicemeeterRemote64\VBVMR_Logout")
	freeRetCode := DllCall("FreeLibrary", "Ptr", VBVMRDLL)

	if (%ErrorLevel% != 0 or %logoutRetCode% != 0 or %freeRetCode% != 0) {	
		MsgBox, 0x10, Error, An error occured.
	}
}

MidiControlChange:
	event := midi.MidiIn()
	mappedValue := map(event.value, 0, 127, -60.0, 12.0)
	command := "Strip[" . faderToStrip[event.controller] . "].Gain"

	DllCall("VoicemeeterRemote64\VBVMR_SetParameterFloat", "AStr", command, "Float", mappedValue)
Return

MidiNoteOn:
	HandleNote(midi.MidiIn(), 0)
Return

MidiNoteOff:
	HandleNote(midi.MidiIn(), 1)
Return