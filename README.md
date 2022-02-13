# ShlagMixer - a DIY midi audio mixer

The point of this project is to control [Voicemeeter Banana](https://vb-audio.com/Voicemeeter/banana.htm) - which is the audio mixer I use during my [Twitch streams](https://twitch.tv/tiphedor) - with a physical controller. This version can control the gain of 5 channels, as well as toggle said channels to 2 outputs. It can be adapted and scaled up/down to suit diffrent needs fairly easily.

## Hardware BOM

 - 10x SPDT toggle switches or similar
 - 5x linear potentiometer / fader, 10k
 - Arduino Leonardo. Pro mini/micro will work too, use the one you like as long as its Atmega32u4-based (must be HID capable to act as a MIDIUSB device)
 - Wires (lol)
 - 3d printer
 
 ## Electronics
 
 Wiring is faril easy, one side of each button to ground, middle (COM) side to digital pins. For the faders, you can use this (very shitty) image: 
 
 ![](/pinout.jpg)
 
 Connect red1 to ground, red2 to an analog pin, and red3 to +5V. You can leave blue1, blue2 and blue3 NC.
 
 Here's a rough schematics, disclamer i've made it after putting the project toghether so it's probably not great :D 
 
 ![](/schematics.png)
