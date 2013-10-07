#include "q3ValueGenerator.h"

unsigned int ValueGenerator::listLen() {
  if (mListLen < 0) {
    throw EndOfFileException();
  }
  return (unsigned int) mListLen;
}

TYPE* ValueGenerator::getValues() {
  // make sure there are values remaingin
  listLen();
  resume();
  return mNewValues;
}

ValuesFromFile::ValuesFromFile(char* filename)
    : ValueGenerator(), mInputFile(filename) {
  if (!mInputFile.is_open()) {
    throw CantOpenInputFileError();
  }
  // get the first list len
  resume();
}

void ValuesFromFile::main() {
  for (;;) {
    mInputFile>>mListLen;
    if (mInputFile.eof()) {
      mListLen = -1;
    }
    suspend();
    mNewValues = new TYPE[mListLen];
    for (int i = 0; i < mListLen; ++i) {
      mInputFile>>mNewValues[i];
    }
  }
}

DefaultValues::DefaultValues(char *filename) {
  ifstream inputFile(filename);
  if (!inputFile.is_open()) {
    throw CantOpenInputFileError();
  }
  inputFile>>mListLen;
}

void DefaultValues::main() {
  mNewValues = new TYPE[mListLen];
  for (int i = 0; i < mListLen; ++i) {
    mNewValues[i] = mListLen - i;
  }
  mListLen = -1;
  suspend();
}
