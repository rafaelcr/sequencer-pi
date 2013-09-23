{
  "targets": [
    {
      "target_name": "midi",
      "sources": [ 
        "midi/midi_connection.cc",
        "midi/midi_note.cc",
        "midi/midi_sequencer.cc",
        "node_module.cc",
        "node_sequence.cc"
      ],
      "libraries": [
        "-lasound"
      ],
    }
  ]
}
