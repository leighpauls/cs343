#include "q1sortmapper.h"
#include "q1kvqueue.h"

SortMapper::SortMapper(
      const string& filename,
      int queueLen,
      int bufferSize,
      uSemaphore* signal)
    : Mapper(filename, queueLen, signal),
      mBufferSize(bufferSize) {}

SortMapper::~SortMapper() {}

void SortMapper::main() {
  // the strings that have been processed
  set<string> processedWords;
  // The buffer to populate the queue from
  map<string, int> buffer;

  for (;;) { // each pass over the document
    ifstream inputFile(getFilename().c_str());
    for (;;) { // each word
      if (readWord(inputFile, processedWords, buffer)) {
        break;
      }
    } // for each word
    if (buffer.empty()) {
      // no new words were picked up this pass, reading is done
      break;
    }
    // flush the buffer to the queue and to processedWords
    for (map<string, int>::iterator it = buffer.begin();
         it != buffer.end();
         ++it) {
      mQueue->pushBack(Mapper::KeyValue(it->first, it->second));
      getSignal()->V();
      processedWords.insert(it->first);
    }
    // empty the buffer
    buffer.clear();
  } // for each pass
  mQueue->close();
  getSignal()->V();
}

/**
 * Read a word into the buffer
 * @param input the file stream to read from
 * @param processedWords the set of words that have already been processed
 * @param buffer the buffer to add the word to
 * @return true iff no word could be read, and the buffer should be flushed
 */
bool SortMapper::readWord(
    istream& input,
    const set<string>& processedWords,
    map<string, int>& buffer) {
  // try to read a word
  string curWord;
  input>>curWord;
  if (input.eof()) {
    return true;
  }
  if (processedWords.count(curWord)) {
    // this word has been sent already
    return false;
  }
  // increment or create the count for that word
  buffer[curWord] += 1;
  if (buffer.size() > (unsigned int) mBufferSize) {
    // I've tried to put too many elements in the buffer
    buffer.erase(buffer.rbegin()->first);
  }
  return false;
}
