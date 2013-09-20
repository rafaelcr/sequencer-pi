#ifndef SEQUENCERPI_MIDI_NOTE_H_
#define SEQUENCERPI_MIDI_NOTE_H_

#include <alsa/asoundlib.h>

#include "midi_connection.h"

class MidiNote {
 public:
  MidiNote();
  MidiNote(MidiConnection *, unsigned char, unsigned char);
  ~MidiNote();
 private:
  MidiConnection *connection_;
  snd_seq_event_t note_on_;
  snd_seq_event_t note_off_;
};

#endif  // SEQUENCERPI_MIDI_NOTE_H_
