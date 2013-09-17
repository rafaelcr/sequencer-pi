#include <alsa/asoundlib.h>

static snd_seq_t *sequencer;
static int in_port;

void midi_open(void)
{
  snd_seq_open(&sequencer, "default", SND_SEQ_OPEN_INPUT, 0);

  snd_seq_set_client_name(sequencer, "Midi Listener");
  in_port = snd_seq_create_simple_port(
      sequencer, 
      "listen:in",
      SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
      SND_SEQ_PORT_TYPE_APPLICATION);

  snd_seq_addr_t sender, dest;
  snd_seq_port_subscribe_t *subs;
  sender.client = 20;
  sender.port = 0;
  dest.client = 128;
  dest.port = 0;
  snd_seq_port_subscribe_alloca(&subs);
  snd_seq_port_subscribe_set_sender(subs, &sender);
  snd_seq_port_subscribe_set_dest(subs, &dest);
  snd_seq_port_subscribe_set_queue(subs, 1);
  snd_seq_port_subscribe_set_time_update(subs, 1);
  snd_seq_port_subscribe_set_time_real(subs, 1);
  snd_seq_subscribe_port(sequencer, subs);
}

snd_seq_event_t *midi_read(void)
{
  snd_seq_event_t *midi_event = NULL;
  snd_seq_event_input(sequencer, &midi_event);
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

int main()
{
  midi_open();
  while (1) {
    midi_process(midi_read());
  }
  return -1;
}
