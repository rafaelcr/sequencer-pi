#include <stdio.h>
#include <alsa/asoundlib.h>

#include "midi_connection.h"
#include "midi_sequencer.h"

#define CHECK(X)  if ((err = (X)) < 0) \
     printf("error %d: %s\n", err, snd_strerror(err));

static MidiConnection *connection;
static MidiSequencer *sequencer;
static int err;

int main() {
  connection = new MidiConnection(20, 0);
  connection->CreateApplicationPort();
  connection->SubscribeOutput();

  sequencer = new MidiSequencer(connection);
  sequencer->CreateEventQueue();

  // TODO(rafaelcr): add some notes, play.
  sequencer->AddNote(0, 0);
  sequencer->AddNote(5, 4);
  sequencer->Play();

  delete sequencer;
  delete connection;
  return 0;
}
