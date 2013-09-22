var app = require('http').createServer(httpHandler)
var io = require('socket.io').listen(app)
var fs = require('fs')
var midi = require('./build/Release/midi');

app.listen(8080);
var sequence = new midi.Sequence(20);

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

io.sockets.on('connection', function(socket) {
  socket.emit('online', null);
  socket.on('client_online', function(data) {
    console.log("Web sequencer online.");
    // TODO(rafaelcr): Remove all previous notes.
  });
  socket.on('note_add', function(data) {
    console.log("Adding note: " + data.note + " step: " + data.step);
    sequence.addNote(parseInt(data.note), parseInt(data.step));
  });
  socket.on('note_remove', function(data) {
    console.log("Removing note: " + data.note + " step: " + data.step);
    sequence.removeNote(parseInt(data.note), parseInt(data.step));
  });
  socket.on('play', function(data) {
    console.log("Playing sequence.");
    sequence.play();
  });
  socket.on('stop', function(data) {
    console.log("Stopping sequence.");
    sequence.stop();
  });
});
