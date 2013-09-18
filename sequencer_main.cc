#include <stdio.h>
#include <alsa/asoundlib.h>

#include "midi_connection.h"

static MidiConnection *midi_connection;

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
  snd_seq_ev_set_source(&ev, 128);
  snd_seq_ev_set_subs(&ev);
  snd_seq_ev_set_direct(&ev);
  snd_seq_ev_set_note(&ev, 
                      0,
                      60,
                      100,
                      20);
  snd_seq_event_output_direct(midi_connection->sequencer(), &ev);
  // snd_seq_drain_output(midi_connection->sequencer());
  printf("output note port:%d\n", midi_connection->application_port());
}

int main() {
  midi_connection = new MidiConnection(20);
  midi_connection->CreateApplicationPort();
  midi_connection->SubscribeInput();
  // while (getchar() != 'q')
  //   deliver_event();
  while (1) {
    midi_process(midi_read());
  }
  return -1;
}
