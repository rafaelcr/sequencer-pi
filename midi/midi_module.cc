#include <node.h>
#include <v8.h>

#include "midi_connection.h"
#include "midi_sequencer.h"

using namespace v8;

static MidiConnection *connection;
static MidiSequencer *sequencer;

Handle<Value> InitSequencer(const Arguments& args) {
  HandleScope scope;
  if (args[0]->IsNumber()) {
    int client = args[0]->Int32Value();

    connection = new MidiConnection(client, 0);
    connection->CreateApplicationPort();
    connection->SubscribeOutput();

    sequencer = new MidiSequencer(connection);
    sequencer->CreateEventQueue();
  }
  return scope.Close(Undefined());
}

Handle<Value> AddNote(const Arguments& args) {
  HandleScope scope;
  if (args[0]->IsNumber() && args[1]->IsNumber()) {
    int note = args[0]->Int32Value();
    int step = args[1]->Int32Value();
    sequencer->AddNote(note, step);
  }
  return scope.Close(Undefined());
}

Handle<Value> Play(const Arguments& args) {
  HandleScope scope;
  sequencer->Play();
  return scope.Close(Undefined());
}

Handle<Value> Stop(const Arguments& args) {
  HandleScope scope;
  sequencer->Stop();
  return scope.Close(Undefined());
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("initSequencer"),
      FunctionTemplate::New(InitSequencer)->GetFunction());
  exports->Set(String::NewSymbol("addNote"),
      FunctionTemplate::New(AddNote)->GetFunction());
  exports->Set(String::NewSymbol("play"),
      FunctionTemplate::New(Play)->GetFunction());
  exports->Set(String::NewSymbol("stop"),
      FunctionTemplate::New(Stop)->GetFunction());
}

NODE_MODULE(midi, init)
