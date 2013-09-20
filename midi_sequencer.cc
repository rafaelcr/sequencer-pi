#include "midi_sequencer.h"

// The default tempo in BPM for the sequencer.
static const int kDefaultTempo = 120;
// The base note (C4). All other notes will be relative to this one.
static const unsigned char kBaseNote = 60;

MidiSequencer::MidiSequencer(MidiConnection *connection)
    : connection_(connection) {
  tempo_ = kDefaultTempo;
}

MidiSequencer::~MidiSequencer() {
  if (queue_) snd_seq_free_queue(connection_->sequencer(), queue_);
}

int MidiSequencer::CreateEventQueue() {
  queue_ = snd_seq_alloc_queue(connection_->sequencer());
}

void MidiSequencer::AddNote(char note, char step) {
  assert(queue_ && "Event queue not initialized.");
  if (note > kNoteMatrixNotes || step > kNoteMatrixSteps) return;
  if (note < 0 || step < 0) return;
  MidiNote *midi_note = new MidiNote(connection_, kBaseNote + note, 127);
  note_matrix_[note][step] = midi_note;
}

void MidiSequencer::RemoveNote(char note, char step) {
  assert(queue_ && "Event queue not initialized.");
  if (note > kNoteMatrixNotes || step > kNoteMatrixSteps) return;
  if (note < 0 || step < 0) return;
  MidiNote *midi_note = note_matrix_[note][step];
  note_matrix_[note][step] = NULL;
  delete midi_note;
}

void MidiSequencer::Play() {
  assert(queue_ && "Event queue not initialized.");
  snd_seq_start_queue(connection_->sequencer(), queue_, NULL);
}

void MidiSequencer::Stop() {
  assert(queue_ && "Event queue not initialized.");
  snd_seq_stop_queue(connection_->sequencer(), queue_, NULL);
}

void MidiSequencer::set_tempo(int tempo) {
  assert(queue_ && "Event queue not initialized.");
  tempo_ = tempo;
  int tempo_micro = tempo_ * 1000000;

  // snd_seq_event_t ev;
  // snd_seq_ev_clear(&ev);
  // ev.dest.client = SND_SEQ_CLIENT_SYSTEM;
  // ev.dest.port = SND_SEQ_PORT_SYSTEM_TIMER;
  // ev.source.client = connection_->application_client();
  // ev.source.port = connection_->application_port();
  // ev.queue = SND_SEQ_QUEUE_DIRECT;
  // ev.data.queue.queue = queue_;
  // ev.data.queue.value = tempo_micro;
  // return snd_seq_event_output(connection_->sequencer(), &ev);
}
