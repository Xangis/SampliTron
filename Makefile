# Sigmatizm makefile.  Requires wxWidgets.
# Uses wxWidgets 2.8.
WX_CONFIG := /usr/bin/wx-config

# Main executable file
PROGRAM = SampliTron

# Directory containing library portions of code.
INCLUDEDIR3 = rtmidi-1.0.11

# Object files
OBJECTS = SettingsDlg.o rtmidi-1.0.11/RtMidi.o wxKeylessChoice.o wxSettingsFile.o wxBitmapSlider.o wxMidiSettingsDlg.o wxVolumeMeter.o wxSwitch.o wxOctaveCtrl.o wxBitmapSpinButton.o wxKeylessBitmapButton.o wxkeyboard.o wxkeyboardapp.o sampledatadlg.o wxADSRDlg.o sampledata.o wavefile.o wxFilterSettingsDlg.o

CXX = $(shell $(WX_CONFIG) --cxx -ggdb)

.SUFFIXES:	.o .cpp

.cpp.o :
#	$(CXX) -c -D__LINUX_ALSASEQ__ -I$(INCLUDEDIR3) `$(WX_CONFIG) --cxxflags` -o $@ $<
	$(CXX) -c -g -ggdb -D__LINUX_ALSASEQ__ -I$(INCLUDEDIR3) `$(WX_CONFIG) --cxxflags` -o $@ $<

all:    $(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) `$(WX_CONFIG) --libs` -lportaudio -lasound -lpthread -lsndfile

clean: 
	rm -f *.o $(PROGRAM)
