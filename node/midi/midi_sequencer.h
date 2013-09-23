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
  // Function that writes a MidiNote to the note matrix. It takes the matrix
  // x and y as arguments, basically, which position the note in time and pitch
  // relative to the base note.
  void AddNote(char, char);
  // Removes a note from the note matrix given its sequencer coordinates.
  void RemoveNote(char, char);
  // Transfers the note matrix into the queue to schedule note on and off
  // MIDI events. This will happen every time the sequence has to begin again.
  void ScheduleSequence();
  void ClearSequence();
  // Playback control methods.
  void Play();
  void Stop();

 private:
  MidiConnection *connection_;
  int queue_;
  int bpm_;
  snd_seq_tick_time_t tick_;
  MidiNote *note_matrix_[kNoteMatrixSteps][kNoteMatrixNotes];
};

#endif  // SEQUENCERPI_MIDI_SEQUENCER_H_
