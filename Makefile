# Sigmatizm makefile.  Requires wxWidgets.
# Uses wxWidgets 2.8 or newer to be on the path.
# If wx-config is not on the path, replace it with the actual path, i.e. /usr/wx/wx-config.
WX_CONFIG := wx-config

# Main executable file
PROGRAM = installer/Samplitron.app/Contents/MacOS/SampliTron

# Directory containing library portions of code.
INCLUDEDIR = ../AudioFile/
INCLUDEDIR2 = ../../lib/libsndfile-1.0.27/include/
INCLUDEDIR3 = ../../lib/rtmidi-4.0.0

# Object files
OBJECTS = SettingsDlg.o $(INCLUDEDIR3)/RtMidi.o ../wxAudioControls/wxKeylessChoice.o ../wxAudioControls/wxSettingsFile.o ../wxAudioControls/wxBitmapSlider.o ../wxAudioControls/wxMidiSettingsDlg.o ../wxAudioControls/wxVolumeMeter.o ../wxAudioControls/wxSwitch.o ../wxAudioControls/wxOctaveCtrl.o ../wxAudioControls/wxBitmapSpinButton.o ../wxAudioControls/wxKeylessBitmapButton.o wxkeyboardapp.o sampledatadlg.o ../wxAudioControls/wxADSRDlg.o sampledata.o ../AudioFile/wavefile.o ../wxAudioControls/wxFilterSettingsDlg.o wxkeyboard.o

CXX = $(shell $(WX_CONFIG) --cxx -ggdb)

.SUFFIXES:	.o .cpp

.cpp.o :
#	$(CXX) -c -D__MACOSX_CORE__ -I$(INCLUDEDIR) -I$(INCLUDEDIR2) -I$(INCLUDEDIR3) `$(WX_CONFIG) --cxxflags` -o $@ $<
	$(CXX) -c -g -ggdb -D__MACOSX_CORE__ -I$(INCLUDEDIR) -I$(INCLUDEDIR2) -I$(INCLUDEDIR3) `$(WX_CONFIG) --cxxflags` -o $@ $<

all:    $(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) `$(WX_CONFIG) --libs` -lportaudio -lpthread -lsndfile -framework CoreMidi -framework CoreAudio

clean: 
	rm -f *.o $(PROGRAM)
