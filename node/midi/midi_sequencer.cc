#include "midi_sequencer.h"

// The default tempo in BPM for the sequencer.
static const int kDefaultBPM = 120;
// The base note (C4). All other notes will be relative to this one.
static const unsigned char kBaseNote = 60;
// Ticks per quarter.
static const int kPPQ = 96;

MidiSequencer::MidiSequencer(MidiConnection *connection)
    : connection_(connection) {
  bpm_ = kDefaultBPM;
  tick_ = 0;
  for (int s = 0; s < kNoteMatrixSteps; s++) {
    for (int n = 0; n < kNoteMatrixNotes; n++) {
      note_matrix_[s][n] = NULL;
    }
  }
}

MidiSequencer::~MidiSequencer() {
  if (queue_ >= 0) {
    snd_seq_free_queue(connection_->sequencer(), queue_);
  }
}

int MidiSequencer::CreateEventQueue() {
  queue_ = snd_seq_alloc_queue(connection_->sequencer());
  // Set default queue tempo.
  snd_seq_queue_tempo_t *tempo;
  snd_seq_queue_tempo_alloca(&tempo);
  snd_seq_queue_tempo_set_tempo(tempo, 60 * 1000000 / bpm_);
  snd_seq_queue_tempo_set_ppq(tempo, kPPQ);
  snd_seq_set_queue_tempo(connection_->sequencer(), queue_, tempo);
  return 1;
}

void MidiSequencer::AddNote(char note, char step) {
  assert((queue_ >= 0) && "Event queue not initialized.");
  if (note > kNoteMatrixNotes || step > kNoteMatrixSteps) return;
  if (note < 0 || step < 0) return;

  MidiNote *midi_note = new MidiNote(connection_, kBaseNote + note, 127);
  note_matrix_[(int)step][(int)note] = midi_note;
}

void MidiSequencer::RemoveNote(char note, char step) {
  assert((queue_ >= 0) && "Event queue not initialized.");
  if (note > kNoteMatrixNotes || step > kNoteMatrixSteps) return;
  if (note < 0 || step < 0) return;

  MidiNote *midi_note = note_matrix_[(int)step][(int)note];
  if (midi_note) {
    delete midi_note;
    note_matrix_[(int)step][(int)note] = NULL;
  }
}

void MidiSequencer::ScheduleSequence() {
  for (int s = 0; s < kNoteMatrixSteps; s++) {
    for (int n = 0; n < kNoteMatrixNotes; n++) {
      MidiNote *note = note_matrix_[s][n];
      if (note) {
        // Schedule both note ON and note OFF events. The note duration will be
        // exactly one 16th note.
        snd_seq_event_t note_on = note->note_on();
        snd_seq_ev_schedule_tick(&note_on, queue_, 0, tick_);
        snd_seq_event_output_direct(connection_->sequencer(), &note_on);
        snd_seq_event_t note_off = note->note_off();
        snd_seq_ev_schedule_tick(&note_off, queue_, 0, tick_ + kPPQ / 4);
        snd_seq_event_output_direct(connection_->sequencer(), &note_off);
      }
    }
    tick_ += kPPQ / 4;
  }
}

void MidiSequencer::ClearSequence() {
  for (int s = 0; s < kNoteMatrixSteps; s++) {
    for (int n = 0; n < kNoteMatrixNotes; n++) {
      MidiNote *midi_note = note_matrix_[s][n];
      if (midi_note) {
        delete midi_note;
      }
      note_matrix_[s][n] = NULL;
    }
  }
  if (queue_ >= 0) {
    snd_seq_remove_events_t *remove_ev;
    snd_seq_remove_events_malloc(&remove_ev);
    snd_seq_remove_events_set_queue(remove_ev, queue_);
    snd_seq_remove_events_set_condition(
        remove_ev, 
        SND_SEQ_REMOVE_OUTPUT | SND_SEQ_REMOVE_IGNORE_OFF);
    snd_seq_remove_events(connection_->sequencer(), remove_ev);
    snd_seq_remove_events_free(remove_ev);
  }
}

void MidiSequencer::Play() {
  assert((queue_ >= 0) && "Event queue not initialized.");
  snd_seq_start_queue(connection_->sequencer(), queue_, NULL);
  snd_seq_drain_output(connection_->sequencer());
  ScheduleSequence();
}

void MidiSequencer::Stop() {
  assert((queue_ >= 0) && "Event queue not initialized.");
  tick_ = 0;
  snd_seq_stop_queue(connection_->sequencer(), queue_, NULL);
  snd_seq_drain_output(connection_->sequencer());
}
