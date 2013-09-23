#include <node.h>

#include "node_sequence.h"

void InitAll(v8::Handle<v8::Object> exports) {
  Sequence::Init(exports);
}

NODE_MODULE(midi, InitAll)
