#ifndef NODE_SEQUENCE_H_
#define NODE_SEQUENCE_H_

#include <node.h>

#include "midi/midi_connection.h"
#include "midi/midi_sequencer.h"

class Sequence : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  Sequence();
  ~Sequence();
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> AddNote(const v8::Arguments& args);
  static v8::Handle<v8::Value> RemoveNote(const v8::Arguments& args);
  static v8::Handle<v8::Value> Play(const v8::Arguments& args);
  static v8::Handle<v8::Value> Stop(const v8::Arguments& args);

  MidiConnection *connection_;
  MidiSequencer *sequencer_;
};

#endif  // NODE_SEQUENCE_H_
