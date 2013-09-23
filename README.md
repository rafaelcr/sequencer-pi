SequencerPi
===========

This library, at the highest level, consists of an interface between a MIDI synthesizer and a web browser using a RaspberryPi as the middleware that serves both MIDI messages to the synthesizer and HTTP messages to the browser. It allows the user to create, play, edit and loop a sequence of notes using a button matrix.

##Layers

SequencerPi consists of 3 main layers:

1. JavaScript user interface: Displays the interactive panel in a simple HTML file and sends HTTP requests through socket.io to the nodejs server notifying any note addition, removal or playback changes.
2. Nodejs server: Connects to the JS interface using a simple HTTP server and socket.io and to the MIDI library using a custom node C++ module.
3. C++ ALSA MIDI library: Handles all the sequencing, queueing and messaging logic between the Linux ALSA and the MIDI device connected to the Raspberry Pi.

##Requirements

This library can be compiled in a RaspberryPi running a Linux distribution like xbian or raspbian. The required packages for compilation are:

* node
* node-gyp
* socket.io
* libasound2-dev
