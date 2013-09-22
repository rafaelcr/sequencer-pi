var app = require('http').createServer(httpHandler)
var io = require('socket.io').listen(app)
var fs = require('fs')
var midi = require('./build/Release/midi');

app.listen(8080);
console.log(midi);

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
    console.log(data);
    midi.initSequencer(20);
    midi.addNote(0, 0);
    midi.play();
  });
});
