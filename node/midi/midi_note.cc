#include "midi_note.h"

static const int kMidiThroughClient = 14;

MidiNote::MidiNote(MidiConnection *connection,
                   unsigned char note, 
                   unsigned char velocity) {
  connection_ = connection;
  // Create note ON event.
  snd_seq_ev_clear(&note_on_);
  snd_seq_ev_set_source(&note_on_, connection_->application_port());
  snd_seq_ev_set_dest(&note_on_, kMidiThroughClient, 0);
  snd_seq_ev_set_fixed(&note_on_);
  note_on_.type = SND_SEQ_EVENT_NOTEON;
  note_on_.data.note.channel = 0;
  note_on_.data.note.note = note;
  note_on_.data.note.velocity = velocity;
  // Create note OFF event.
  snd_seq_ev_clear(&note_off_);
  snd_seq_ev_set_source(&note_off_, connection_->application_port());
  snd_seq_ev_set_dest(&note_off_, kMidiThroughClient, 0);
  snd_seq_ev_set_fixed(&note_off_);
  note_off_.type = SND_SEQ_EVENT_NOTEOFF;
  note_off_.data.note.channel = 0;
  note_off_.data.note.note = note;
  note_off_.data.note.velocity = velocity;
}
