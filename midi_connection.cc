#include "midi_connection.h"

static const int kMidiThroughClientNumber = 14;
static const char* kClientName = "SequencerPi";

MidiConnection::MidiConnection(int device_client, int device_port) 
    : device_client_(device_client),
      device_port_(device_port) {}

void MidiConnection::CreateApplicationPort() {
  snd_seq_open(&sequencer_, "default", SND_SEQ_OPEN_DUPLEX, 0);
  snd_seq_set_client_name(sequencer_, kClientName);
  application_client_ = snd_seq_client_id(sequencer_);
  application_port_ = snd_seq_create_simple_port(
      sequencer_, 
      kClientName,
      SND_SEQ_PORT_CAP_READ 
        | SND_SEQ_PORT_CAP_SUBS_READ
        | SND_SEQ_PORT_CAP_WRITE 
        | SND_SEQ_PORT_CAP_SUBS_WRITE,
      SND_SEQ_PORT_TYPE_APPLICATION);
}

int MidiConnection::SubscribeInput() {
  assert(sequencer_ && "Application port not initialized.");
  snd_seq_addr_t sender, dest;
  snd_seq_port_subscribe_t *subs;
  sender.client = device_client_;
  sender.port = device_port_;
  dest.client = application_client_;
  dest.port = 0;
  snd_seq_port_subscribe_alloca(&subs);
  snd_seq_port_subscribe_set_sender(subs, &sender);
  snd_seq_port_subscribe_set_dest(subs, &dest);
  snd_seq_port_subscribe_set_queue(subs, 1);
  snd_seq_port_subscribe_set_time_update(subs, 1);
  snd_seq_port_subscribe_set_time_real(subs, 1);
  snd_seq_subscribe_port(sequencer_, subs);

  return 1;
}

int MidiConnection::SubscribeOutput() {
  assert(sequencer_ && "Application port not initialized.");
  snd_seq_addr_t sender, dest;
  snd_seq_port_subscribe_t *subs;
  sender.client = kMidiThroughClientNumber;
  sender.port = 0;
  dest.client = device_client_;
  dest.port = device_port_;
  snd_seq_port_subscribe_alloca(&subs);
  snd_seq_port_subscribe_set_sender(subs, &sender);
  snd_seq_port_subscribe_set_dest(subs, &dest);
  snd_seq_subscribe_port(sequencer_, subs);

  return 1;
}
