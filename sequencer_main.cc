#include <stdio.h>
#include <alsa/asoundlib.h>

#include "midi_connection.h"

#define CHECK(X)  if ((err = (X)) < 0) \
     printf("error %d: %s\n", err, snd_strerror(err));

static MidiConnection *midi_connection;
static int err;

snd_seq_event_t *midi_read(void)
{
  snd_seq_event_t *midi_event = NULL;
  snd_seq_event_input(midi_connection->sequencer(), &midi_event);
  return midi_event;
}

void midi_process(snd_seq_event_t *ev)
{
  switch (ev->type) {
    case SND_SEQ_EVENT_NOTEON:
    case SND_SEQ_EVENT_NOTEOFF: {
      const char *type = (ev->type == SND_SEQ_EVENT_NOTEON) ? "on " : "off";

      printf("[%d] Note %s: %2x vel(%2x)\n", 
             ev->time.tick, type,
             ev->data.note.note,
             ev->data.note.velocity);
      break;
    }
    case SND_SEQ_EVENT_CONTROLLER: {
      printf("[%d] Control:  %2x val(%2x)\n", 
             ev->time.tick,
             ev->data.control.param,
             ev->data.control.value);
      break;
    }
    default: {
      break;
    }
  }
}

void deliver_event() {
  snd_seq_event_t ev;
  snd_seq_ev_clear(&ev);
  snd_seq_ev_set_source(&ev, midi_connection->application_port());
  snd_seq_ev_set_dest(&ev, 14, 0);
  snd_seq_ev_set_fixed(&ev);
  snd_seq_ev_set_direct(&ev);
  ev.type = SND_SEQ_EVENT_NOTEON;
  ev.data.note.channel = 0;
  ev.data.note.note = 64 ;
  ev.data.note.velocity = 127;
  CHECK(snd_seq_event_output_direct(midi_connection->sequencer(), &ev));
  sleep(1);
  ev.type = SND_SEQ_EVENT_NOTEOFF;
  CHECK(snd_seq_event_output_direct(midi_connection->sequencer(), &ev));
}

int main() {
  midi_connection = new MidiConnection(20, 0);
  midi_connection->CreateApplicationPort();
  midi_connection->SubscribeOutput();
  // while (getchar() != 'q')
    deliver_event();
  // while (1) {
  //   midi_process(midi_read());
  // }
  return -1;
}
