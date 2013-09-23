var app = require('http').createServer(httpHandler)
var io = require('socket.io').listen(app)
var fs = require('fs')
var midi = require('./build/Release/midi');

// HTTP web server will listen on port 8080 to avoid running as root.
app.listen(8080);
// The MIDI sequencer will communicate with the device on port 20 by default.
var sequence = new midi.Sequence(20);

// Simple web server implementation.
// Receives requests from socket.io and responds with the appropiate file from
// the web directory. This will only serve simple static pages, CSS
// and JS files.
function httpHandler(req, res) {
  var filename = (req.url == '/') ? 'index.html' : req.url;
  fs.readFile(__dirname + '/../web/' + filename,
  function (err, data) {
    if (err) {
      res.writeHead(500);
      return res.end('Error loading ' + filename);
    }
    res.writeHead(200);
    res.end(data);
  });
}

// Socket connection and message handlers.
io.sockets.on('connection', function(socket) {
  // Emit 'online' status when client is connected to acknowledge the channel.
  socket.emit('online', null);

  // Clear the sequencer when a new web session is initiated. Naturally, only
  // one sequencer can run at a time.
  socket.on('client_online', function(data) {
    console.log("Web sequencer online.");
    sequence.clear();
  });

  // Adds a note to the sequence.
  socket.on('note_add', function(data) {
    console.log("Adding note: " + data.note + " step: " + data.step);
    sequence.addNote(parseInt(data.note), parseInt(data.step));
  });

  // Removes a note from the sequence.
  socket.on('note_remove', function(data) {
    console.log("Removing note: " + data.note + " step: " + data.step);
    sequence.removeNote(parseInt(data.note), parseInt(data.step));
  });

  // Plays the sequence with the current configured MIDI notes.
  socket.on('play', function(data) {
    console.log("Playing sequence.");
    sequence.play();
  });

  // Stops playback.
  socket.on('stop', function(data) {
    console.log("Stopping sequence.");
    sequence.stop();
  });

  // Loops the current sequence and plays again.
  socket.on('loop', function(data) {
    sequence.stop();
    sequence.play();
  });
});
