#pragma once

#include "q1mapper.h"
#include <fstream>

#include <set>
#include <map>

using namespace std;

_Task SortMapper : public Mapper {
public:
  // Writes items to the queue in sorted order.
  SortMapper(
      const string& filename,
      int queueLen,
      int bufferSize,
      uSemaphore* signal);
  virtual ~SortMapper();

protected:
  virtual void main();

private:
  int mBufferSize;

  bool readWord(
      istream& input,
      const set<string>& processedWords,
      map<string, int>& buffer);
};
