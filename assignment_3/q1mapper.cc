#include "q1mapper.h"
#include "q1kvqueue.h"

#include <fstream>

using namespace std;

Mapper::Mapper(const string& filename, int queueLen, uSemaphore* signal)
    : mQueue(new KVQueue(queueLen)), mFilename(filename), mSignal(signal) { }

Mapper::~Mapper() {
  delete mQueue;
}

uSemaphore* Mapper::getSignal() {
  return mSignal;
}

const string& Mapper::getFilename() {
  return mFilename;
}

void Mapper::main() {
  ifstream inputFile(mFilename.c_str());
  string word;
  for (;;) {
    // try to read a word
    inputFile>>word;
    if (inputFile.eof()) {
      break;
    }
    // make the new mapping
    KeyValue kv(word, 1);
    mQueue->pushBack(kv);
    // Tell the reducers that there's a new value
    mSignal->V();
  }
  mQueue->close();
  // tell the reducers that a mapper has closed
  mSignal->V();
}
