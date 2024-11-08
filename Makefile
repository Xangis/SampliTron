# Sigmatizm makefile.  Requires wxWidgets.
# Uses wxWidgets 2.8 or newer to be on the path.
# If wx-config is not on the path, replace it with the actual path, i.e. /usr/wx/wx-config.
WX_CONFIG := wx-config

# Main executable file
PROGRAM = installer/Samplitron.app/Contents/MacOS/SampliTron

# Directory containing library portions of code.
INCLUDEDIR = ../AudioFile/
INCLUDEDIR2 = ../../lib/libsndfile-1.0.28/src
INCLUDEDIR3 = ../../lib/rtmidi-4.0.0
LIBDIR = ../../lib/libsndfile-1.0.28/src/.libs/

# Object files
OBJECTS = SettingsDlg.o $(INCLUDEDIR3)/RtMidi.o ../wxAudioControls/wxKeylessChoice.o ../wxAudioControls/wxSettingsFile.o ../wxAudioControls/wxBitmapSlider.o ../wxAudioControls/wxMidiSettingsDlg.o ../wxAudioControls/wxVolumeMeter.o ../wxAudioControls/wxSwitch.o ../wxAudioControls/wxOctaveCtrl.o ../wxAudioControls/wxBitmapSpinButton.o ../wxAudioControls/wxKeylessBitmapButton.o sampledatadlg.o ../wxAudioControls/wxADSRDlg.o sampledata.o ../AudioFile/wavefile.o ../wxAudioControls/wxFilterSettingsDlg.o Samplitron.o SamplitronApp.o

CXX = $(shell $(WX_CONFIG) --cxx -ggdb)

.SUFFIXES:	.o .cpp

.cpp.o :
#	$(CXX) -c -D__MACOSX_CORE__ -I$(INCLUDEDIR) -I$(INCLUDEDIR2) -I$(INCLUDEDIR3) `$(WX_CONFIG) --cxxflags` -o $@ $<
	$(CXX) -c -g -ggdb -D__MACOSX_CORE__ -I$(INCLUDEDIR) -I$(INCLUDEDIR2) -I$(INCLUDEDIR3) `$(WX_CONFIG) --cxxflags` -o $@ $<

all:    $(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) -L$(LIBDIR) `$(WX_CONFIG) --libs` -lpthread -lsndfile -framework CoreMidi -framework CoreAudio

clean: 
	rm -f *.o ../wxAudioControls/*.o ../AudioFile/*.o $(PROGRAM)
