# IMPORTANT
# Use this makefile only when testing this MIDI component individually.
# For building the node library use node-gyp.
CC=g++
CFLAGS=-g
LDFLAGS=-lasound
SOURCES=main.cc midi_connection.cc midi_sequencer.cc midi_note.cc
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=sequencerpi.out

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(EXECUTABLE)
