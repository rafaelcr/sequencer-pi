#include <node.h>

#include "node_sequence.h"

Sequence::Sequence() {};
Sequence::~Sequence() {};

void Sequence::Init(v8::Handle<v8::Object> exports) {
  // Prepare constructor template.
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
  tpl->SetClassName(v8::String::NewSymbol("Sequence"));
  tpl->InstanceTemplate()->SetInternalFieldCount(2);
  // Prototype
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("addNote"),
      v8::FunctionTemplate::New(AddNote)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("removeNote"),
      v8::FunctionTemplate::New(RemoveNote)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("clear"),
      v8::FunctionTemplate::New(Clear)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("play"),
      v8::FunctionTemplate::New(Play)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("stop"),
      v8::FunctionTemplate::New(Stop)->GetFunction());

  v8::Persistent<v8::Function> constructor = 
      v8::Persistent<v8::Function>::New(tpl->GetFunction());
  exports->Set(v8::String::NewSymbol("Sequence"), constructor);
}

v8::Handle<v8::Value> Sequence::New(const v8::Arguments& args) {
  v8::HandleScope scope;

  Sequence* seq = new Sequence();
  if (args[0]->IsNumber()) {
    int client = args[0]->Int32Value();

    seq->connection_ = new MidiConnection(client, 0);
    seq->connection_->CreateApplicationPort();
    seq->connection_->SubscribeOutput();

    seq->sequencer_ = new MidiSequencer(seq->connection_);
    seq->sequencer_->CreateEventQueue();
  }
  seq->Wrap(args.This());

  return args.This();
}

v8::Handle<v8::Value> Sequence::AddNote(const v8::Arguments& args) {
  v8::HandleScope scope;
  Sequence* seq = ObjectWrap::Unwrap<Sequence>(args.This());

  if (args[0]->IsNumber() && args[1]->IsNumber()) {
    int note = args[0]->Int32Value();
    int step = args[1]->Int32Value();
    seq->sequencer_->AddNote(note, step);
  }

  return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> Sequence::RemoveNote(const v8::Arguments& args) {
  v8::HandleScope scope;
  Sequence* seq = ObjectWrap::Unwrap<Sequence>(args.This());

  if (args[0]->IsNumber() && args[1]->IsNumber()) {
    int note = args[0]->Int32Value();
    int step = args[1]->Int32Value();
    seq->sequencer_->RemoveNote(note, step);
  }

  return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> Sequence::Clear(const v8::Arguments& args) {
  v8::HandleScope scope;
  Sequence* seq = ObjectWrap::Unwrap<Sequence>(args.This());
  seq->sequencer_->ClearSequence();
  return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> Sequence::Play(const v8::Arguments& args) {
  v8::HandleScope scope;
  Sequence* seq = ObjectWrap::Unwrap<Sequence>(args.This());
  seq->sequencer_->Play();
  return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> Sequence::Stop(const v8::Arguments& args) {
  v8::HandleScope scope;
  Sequence* seq = ObjectWrap::Unwrap<Sequence>(args.This());
  seq->sequencer_->Stop();
  return scope.Close(v8::Undefined());
}
