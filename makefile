CC=g++
CFLAGS=-g
LDFLAGS=-lasound
SOURCES=sequencer_main.cc midi_connection.cc
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=sequencerpi.out

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(EXECUTABLE)
