#ifndef SEQUENCERPI_MIDI_NOTE_H_
#define SEQUENCERPI_MIDI_NOTE_H_

#include <alsa/asoundlib.h>

#include "midi_connection.h"

class MidiNote {
 public:
  MidiNote();
  MidiNote(MidiConnection *, unsigned char, unsigned char);
  ~MidiNote();
  snd_seq_event_t note_on() const { return note_on_; }
  snd_seq_event_t note_off() const { return note_off_; }

 private:
  MidiConnection *connection_;
  snd_seq_event_t note_on_;
  snd_seq_event_t note_off_;
  char step_;
};

#endif  // SEQUENCERPI_MIDI_NOTE_H_
