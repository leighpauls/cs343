#pragma once

#include <fstream>
#include <stdexcept>

using namespace std;

/// Absract generator for input values
_Coroutine ValueGenerator {
public:
  ValueGenerator() {};
  virtual ~ValueGenerator() {}

  TYPE* getValues();
  // throws EndOfFileException when done
  unsigned int listLen();
  class EndOfFileException {};
protected:
  virtual void main() = 0;

  // coroutine output variables
  int mListLen;
  TYPE* mNewValues;
};

/// Read a lists of values from a file, for "mode 1"
_Coroutine ValuesFromFile : public ValueGenerator {
public:
  ValuesFromFile(char *filename);
protected:
  virtual void main();
private:
  ifstream mInputFile;
};

/// Make a reverse list of values from a file for "mode 2"
_Coroutine DefaultValues : public ValueGenerator {
public:
  DefaultValues(char *filename);
protected:
  virtual void main();
};
