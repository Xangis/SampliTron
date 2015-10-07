# SampliTron

A virtual keyboard sampler for Windows. It was originally released at http://zetacentauri.com

![Samplitron Screenshot](https://github.com/Xangis/SampliTron/blob/master/images/Samplitron.png)

SampliTron is a standalone software sampler designed for live play. It does not need a plugin 
host application and no additional software or hardware is required to run it. It uses the 
DirectX XAudio2 libraries and should work on any Windows computer less than 7 years old.

SampliTron is ideal for musicians who want to add samples and sound effects to live 
performances without the complexity and expense of a complicated hardware sampler setup.

Beginners new to the creation of sample-based music should also find this to be a useful 
tool for experimentation and learning. SampliTron can be used quickly, without needing to 
read a complicated two-inch-thick manual.

Features

- Control via external MIDI device.
- Notes can be played using the computer keyboard.
- Can generate MIDI controller data and be used to control external devices.
- Keyboard configurable to show from 3 to 7 octaves.
- Allows detailed splits with a different sample for each key.
- Allows using .WAV samples of different bit depths and sample rates.
- Automatically scales pitch of loaded samples.
- Auomatically generates keyboard splits.
- Easy to see which keys have samples loaded by pressing the ALT key.

Getting Started

It is very easy to use. To get started, right click on one of the keys to load a sample.
Click "Browse" to select a .wav file and click "OK" when you've selected one.

![SampliTron Sample Settings](https://github.com/Xangis/SampliTron/blob/master/images/SamplitronSampleSettings.png)

Once a sample is loaded, you can play notes using the keys of your computer keyboard, with 
each row of keys playing the notes of a different octave. You can also load multi-sample 
"patches" by clicking the folder icon. There are a few of them bundled with the program, 
and with the registered version you can create as many setups as you like for quick recall.

### MIDI

If you have an external MIDI controller, SampliTron will respond to note data. To change 
MIDI settings, you can click on the circular MIDI connector icon.

![SampliTron MIDI Settings](https://github.com/Xangis/SampliTron/blob/master/images/SamplitronMidi.png)

You can use any MIDI input device connected to your computer to trigger SampliTron. Be 
sure to set the input device and channel to match your MIDI controller's connections. 
To use SampliTron as a MIDI controller like you can with Proxima Controller, check the 
"MIDI Output Enabled" button. If it's lit, you'll be transmitting note data to the 
selected MIDI out device when you trigger samples with your computer keyboard.

### Problems

It will not currently build. I'm still tracking down some files needed by the program.
