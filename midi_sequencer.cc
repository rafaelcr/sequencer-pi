#include "midi_sequencer.h"

static const int kDefaultTempo = 120;

MidiSequencer::MidiSequencer(MidiConnection *connection)
    : connection_(connection) {
  tempo_ = kDefaultTempo;
}

MidiSequencer::~MidiSequencer() {
  snd_seq_free_queue(connection_->sequencer(), queue_);
}

int MidiSequencer::CreateEventQueue() {
  queue_ = snd_seq_alloc_queue(connection_->sequencer());
}

void MidiSequencer::ScheduleNote(int key, int velocity, int duration) {
  
}

void MidiSequencer::Play() {
  snd_seq_start_queue(connection_->sequencer(), queue_, NULL);
}

void MidiSequencer::Stop() {
  snd_seq_stop_queue(connection_->sequencer(), queue_, NULL);
}

void MidiSequencer::set_tempo(int tempo) {
  assert(queue_ && "Event queue not initialized.");
  tempo_ = tempo;
  int tempo_micro = tempo_ * 1000000;

  snd_seq_event_t ev;
  snd_seq_ev_clear(&ev);
  ev.dest.client = SND_SEQ_CLIENT_SYSTEM;
  ev.dest.port = SND_SEQ_PORT_SYSTEM_TIMER;
  ev.source.client = connection_->application_client();
  ev.source.port = connection_->application_port();
  ev.queue = SND_SEQ_QUEUE_DIRECT;
  ev.data.queue.queue = queue_;
  ev.data.queue.value = tempo_micro;
  return snd_seq_event_output(connection_->sequencer(), &ev);
}
