#include "midi_connection.h"

static const int kApplicationPortNumber = 128;
static const char* kClientName = "SequencerPi";
static const char* kPortName = "ApplicationPort";

MidiConnection::MidiConnection(int device_port) : device_port_(device_port) {}

void MidiConnection::CreateApplicationPort() {
  snd_seq_open(&sequencer_, "default", SND_SEQ_OPEN_INPUT, 0);
  snd_seq_set_client_name(sequencer_, kClientName);
  application_port_ = snd_seq_create_simple_port(
      sequencer_, 
      kPortName,
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
  sender.client = device_port_;
  sender.port = 0;
  dest.client = kApplicationPortNumber;
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
  sender.client = kApplicationPortNumber;
  sender.port = 0;
  dest.client = device_port_;
  dest.port = 1;
  snd_seq_port_subscribe_alloca(&subs);
  snd_seq_port_subscribe_set_sender(subs, &sender);
  snd_seq_port_subscribe_set_dest(subs, &dest);
  snd_seq_subscribe_port(sequencer_, subs);

  return 1;
}
