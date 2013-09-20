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
