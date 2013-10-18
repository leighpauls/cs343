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
    inputFile>>word;
    if (inputFile.eof()) {
      break;
    }
    KeyValue kv(word, 1);
    mQueue->pushBack(kv);
    mSignal->V();
  }
  mQueue->close();
}
