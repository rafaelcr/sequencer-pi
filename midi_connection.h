#ifndef SEQUENCERPI_MIDI_CONNECTION_H_
#define SEQUENCERPI_MIDI_CONNECTION_H_

#include <alsa/asoundlib.h>

class MidiConnection {
 public:
  MidiConnection();
  MidiConnection(int, int);

  // Creates an application port for us to read and write from a MIDI device.
  void CreateApplicationPort();

  // Subscribe to a MIDI device to read events from it.
  int SubscribeInput();

  // Subscribe to a MIDI device to write events to it.
  int SubscribeOutput();

  // Getter methods.
  snd_seq_t* sequencer() const { return sequencer_; }
  int application_client() const { return application_client_; }
  int application_port() const { return application_port_; }
  int device_client() const { return device_client_; }
  int device_port() const { return device_port_; }

 private:
  snd_seq_t *sequencer_;
  int application_client_;
  int application_port_;
  int device_client_;
  int device_port_;
};

#endif  // SEQUENCERPI_MIDI_CONNECTION_H_
