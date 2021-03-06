# ShlagMixer - a DIY midi audio mixer

![](/pics/finishedProduct.jpg)

## Table of Contents

  - [Hardware](#hardware)
    + [BOM](#bom)
    + [Electronics](#electronics)
  - [Software](#software)
    + [Arduino](#arduino)
    + [AutoHotKey](#autohotkey)
  - [3D printed parts](#3d-printed-parts)

--- 

The point of this project is to control [Voicemeeter Banana](https://vb-audio.com/Voicemeeter/banana.htm) - which is the audio mixer I use during my [Twitch streams](https://twitch.tv/tiphedor) - with a physical controller. This version can control the gain of 5 channels, as well as toggle said channels to 2 outputs. It can be adapted and scaled up/down to suit different needs fairly easily.

## Hardware

### BOM

 - 10x SPDT toggle switches or similar
 - 5x linear potentiometer / fader, 10k
 - Arduino Leonardo. Pro mini/micro will work too, use the one you like as long as it's Atmega32u4-based (must be HID capable to act as a MIDIUSB device)
 - Wires (lol)
 - 3d printer
 
### Electronics
 
Wiring is fairly easy, one side of each button goes to ground, middle sides (COM) to digital pins D4-D13. For the faders, you can use this (very shitty) image: 
 
![](/pics/pinout.jpg)
 
Connect red1 to ground, red2 to A0-A4, and red3 to +5V. You can leave blue1, blue2 and blue3 NC.
 
Here's a rough schematic, disclaimer I've drawn it after putting the project together, so it's probably not very accurate :D 
 
![](/pics/schematics.png)

## Software

### Arduino

Open the project with [PlatformIO](https://platformio.org/), run : 

```bash
pio lib install
pio run
```

You should see a new device connected to your computer. I suggest using [MidiView](https://hautetechnique.com/midi/midiview/) to make sure the data is being received as you'd expect.

### AutoHotKey

Download and install [AutoHotKey](https://www.autohotkey.com/), then right-click the `main.ahk` file, click on "compile", which should produce a binary, `main.exe`. Run the binary - profit ! You can make the script run at startup by [following the official documentation](https://www.autohotkey.com/docs/FAQ.htm#Startup).

## 3D printed parts

You will need to print the mixer itself `shlagMixer.stl`, as well as 5 faders knobs `faderKnob.stl`. You can use whatever settings you'd like, neither are very hard to print. Mine were printed on a Prusa Mk3s, Prusament Black PLA, default profile, 0.2mm layer height. No supports required, and no raft either.
