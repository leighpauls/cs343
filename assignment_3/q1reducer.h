#pragma once

#include "q1mapper.h"

#include <map>
#include <vector>

using namespace std;

_Task Reducer {
public:
  Reducer(
      int id,
      int numReducers,
      uSemaphore* signal,
      const vector<Mapper*>& mappers);
  virtual ~Reducer();
  _Nomutex static unsigned long hash(const string& str);
  int getID();
  int getNumReducers();
  uSemaphore* getSignal();
  vector<Mapper*>& getMappers();

protected:
  void main();

private:
  int mId, mNumReducers;
  uSemaphore *mSignal;
  vector<Mapper*> mMappers;
  map<string, int> mWordCounts;

  void reduce(Mapper::KeyValue& pair);
  void printCounts();
};
