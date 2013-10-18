#pragma once

#include <uSemaphore.h>

using namespace std;

struct KVQueue;

_Task Mapper {
public:
  struct KeyValue {
    string key;
    int value;

    KeyValue(const string& key, int value) : key(key), value(value) {}
    KeyValue() {}
  };
  Mapper(const string& filename, int queueLen, uSemaphore* signal);
  virtual ~Mapper();
  uSemaphore* getSignal();
  const string& getFilename();

  KVQueue* mQueue;

protected:
  void main();

private:
  string mFilename;
  uSemaphore* mSignal;
};
