# SampliTron

A virtual keyboard sampler for Windows. It was originally released as commercial software at 
http://zetacentauri.com and has been downloaded more than 40,000 times (including demo 
versions).

![Samplitron Screenshot](https://github.com/Xangis/SampliTron/blob/master/images/Samplitron.png)

SampliTron is a standalone software sampler designed for live play. It does not need a plugin 
host application and no additional software or hardware is required to run it. It uses the 
DirectX XAudio2 libraries and should work on any Windows computer less than 7 years old.

SampliTron is ideal for musicians who want to add samples and sound effects to live 
performances without the complexity and expense of a complicated hardware sampler setup.

Beginners new to the creation of sample-based music should also find this to be a useful 
tool for experimentation and learning. SampliTron can be used quickly, without needing to 
read a complicated two-inch-thick manual.

You can download a pre-built Windows installer here:

https://zetacentauri.com/software/SampliTron1.1Setup.exe

## Features

- Control via external MIDI device.
- Notes can be played using the computer keyboard.
- Can generate MIDI controller data and be used to control external devices.
- Keyboard configurable to show from 3 to 7 octaves.
- Allows detailed splits with a different sample for each key.
- Allows using .WAV samples of different bit depths and sample rates.
- Automatically scales pitch of loaded samples.
- Auomatically generates keyboard splits.
- Easy to see which keys have samples loaded by pressing the ALT key.

## Getting Started

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

## Building

This project depends on the wxAudioControls and AudioFile repositories, also available via my
GitHub account:

https://github.com/xangis/AudioFile

https://github.com/xangis/wxAudioControls

They should be checked out into the same parent folder as SampliTron, so you have "AudioFile",
"SampliTron", and "wxAudioControls" directories in the same folder (as neighbors).

### Windows

This is meant to be built with Visual Studio. Use the SampliTron.sln solution file in the
root folder.

You will probably need to edit library paths to point to the directories where you've placed
dependencies.

Once the app is built, you can use InnoSetup (free) to build a Windows installer. Use the file
installer/Samplitron1.2Installer.iss to build the installer package.

There was an attempt to package this app for the Windows Store. See installer/package.bat
for what had been done so far. That was never quite finished.

### Linux

On Linux, this app requires wxWdgets 2.8 or newer (preferably 3.0), libportaudio-dev, 
ibasound2-dev, and libsndfile-dev.

On linux, you can get requirements with:

sudo apt-get install libwxgtk3.0-dev librtmidi-dev libsndfile1-dev libportaudio-dev

Build with this command:

make -f Makefile.linux

### OSX

You'll need to have g++ available on the command line.

Build with this command:

make

There was a failed attempt to package this app for the Apple Store. The file
installer/OSXLibraryPaths.sh is meant to fix library paths, while installer/package.sh
is meant to create an app bundle. These probably need some work in order to do the right
thing.

### Problems

There are currently some build path issues that need to be resolved, so you may have trouble
building on your system. I am willing to accept pull requests if you have fixes to contribute,
otherwise they'll be addressed as I have time.

The same goes for packing the executable.

On Linux, it will build but not produce sound (and the UI looks messed up). This may no longer
be true -- test for yourself and let me know.

#### OSX Issues / TODO

The sound level meter does not work.

The XPM icons don't look good.

Resizing to 4 octaves doesn't shrink the window size.


