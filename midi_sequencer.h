#ifndef SEQUENCERPI_MIDI_SEQUENCER_H_
#define SEQUENCERPI_MIDI_SEQUENCER_H_

#include <alsa/asoundlib.h>

#include "midi_connection.h"
#include "midi_note.h"

static const int kNoteMatrixSteps = 16;
static const int kNoteMatrixNotes = 16;

class MidiSequencer {
 public:
  MidiSequencer();
  MidiSequencer(MidiConnection *);
  ~MidiSequencer();

  // Creates the event queue for scheduling notes, etc.
  int CreateEventQueue();

  // Note scheduling methods.
  void ScheduleNote(int, int, int);

  // Playback control methods.
  void Play();
  void Stop();

  // Setter methods, usually external interfaces for real time interaction.
  void set_tempo(int);
 private:
  MidiConnection *connection_;
  int queue_;
  int tempo_;
  MidiNote *note_matrix_[kNoteMatrixSteps][kNoteMatrixNotes];
};

#endif  // SEQUENCERPI_MIDI_SEQUENCER_H_
