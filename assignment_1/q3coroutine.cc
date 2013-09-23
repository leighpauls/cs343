#include <iostream>
#include <fstream>
#include <stdexcept>
#include <iomanip>

#include "q3utf8decoder.h"

using namespace std;

/**
 * Convert from an array of bytes to an int
 * @param  bytes  char array to read from
 * @param  len    how many bytes to read
 * @return        unsinged int representation of bytes
 */
static unsigned int fromByteArray(unsigned char *bytes, size_t len) {
  unsigned int res = 0;
  for (size_t i = 0; i < len; i++) {
    res = (res << 8) | bytes[i];
  }
  return res;
}

/**
 * Helper method for formatting the output like the example program
 * @param buffer    buffer of bytes that was decoded
 * @param consumed  number of bytes consumed by the decoding
 * @param inputLen  number of bytes actually put into buffer
 * @param success   true iff the decoding was successful
 * @param unicode   the decoded value
 */
static void writeResultLine(
    unsigned char *buffer,
    size_t consumed,
    size_t inputLen,
    bool success,
    unsigned int unicode = 0) {
  if (consumed > 0) {
    cout << "0x" << hex << fromByteArray(buffer, consumed);
  }
  cout<<" : ";
  if (inputLen == 0) {
    cout << "Warning! Blank line." << endl;
    return;
  }
  if (success) {
    cout << "valid 0x" << hex << unicode;
  } else {
    cout << "invalid";
  }
  if (consumed != inputLen) {
    cout << ". Extra characters 0x"
         << hex << fromByteArray(&buffer[consumed], inputLen - consumed);
  }
  cout << endl;
}

/**
 * Attempts to decode one line as utf8
 * @param buffer  bytes to decode
 * @param limit   number of bytes available in buffer
 */
static void utf8DecodeLine(unsigned char *buffer, size_t limit) {
  Utf8 decoder;
  int i;
  for (i = 0; i < limit; ++i) {
    try {
      decoder.next(buffer[i]);
    } catch (Utf8::Match match) {
      writeResultLine(buffer, i+1, limit, true, match.unicode);
      return;
    } catch (Utf8::Error error) {
      writeResultLine(buffer, i+1, limit, false);
      return;
    }
  }
  // If the decoder gets here, then it has failed to find a unicode
  writeResultLine(buffer, i, limit, false);
}

/**
 * Reads the stream line-by-line, outputing what it can decode, or if it can't.
 * Exits when the stream has been exausted.
 * @param in  stream to read from
 */
static void utf8DecodeStream(istream &in) {
  const size_t BUFFER_SIZE = 32;
  unsigned char buffer[BUFFER_SIZE];
  for (;;) {
    in.getline((char*)buffer, BUFFER_SIZE);
    if (in.eof()) {
      // I've reached the end of the file safely
      return;
    }
    if (in.fail()) {
      throw runtime_error(
          "Didn't allocate enough buffer space to read this line");
    }
    utf8DecodeLine(buffer, in.gcount() - 1);
  }
}

/**
 * Immitation main function, broken out so that runtime_error's are prettier to
 * catch and log.
 */
static void doUtf8Decode(int argc, char** argv) {
  if (argc <= 1) {
    utf8DecodeStream(cin);
  } else {
    ifstream input_file;
    input_file.open(argv[1], ios::in | ios::binary);

    if (input_file.is_open()) {
      utf8DecodeStream(input_file);
    } else {
      throw runtime_error("couldn't open the input file");
    }
  }
}

void uMain::main() {
  try {
    doUtf8Decode(argc, argv);
  } catch (runtime_error e) {
    cout<<"RUNTIME ERROR NOT CAUGHT: "<<e.what()<<endl;
    throw;
  }
}
