#include <stdio.h>
#include <alsa/asoundlib.h>

#include "midi_connection.h"
#include "midi_sequencer.h"

static MidiConnection *connection;
static MidiSequencer *sequencer;

// Tests the ALSA MIDI connection and sequencer.
int main() {
  connection = new MidiConnection(20, 0);
  connection->CreateApplicationPort();
  connection->SubscribeOutput();

  sequencer = new MidiSequencer(connection);
  sequencer->CreateEventQueue();

  sequencer->AddNote(0, 0);
  sequencer->AddNote(3, 0);
  sequencer->AddNote(5, 1);
  sequencer->Play();

  getchar();
  delete sequencer;
  delete connection;
  return 0;
}
