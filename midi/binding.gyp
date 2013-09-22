{
  "targets": [
    {
      "target_name": "midi",
      "sources": [ 
        "midi_connection.cc",
        "midi_module.cc",
        "midi_note.cc",
        "midi_sequencer.cc" 
      ],
      "libraries": [
        "-lasound"
      ],
    }
  ]
}
