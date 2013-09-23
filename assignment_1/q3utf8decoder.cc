#include "q3utf8decoder.h"
#include <stdexcept>

using namespace std;

/**
 * Gives the decoder another byte to consume
 * @param c  The new byte to decode
 * @throws Error  when c makes the code invalid
 * @throws Match  when c completes a valid code
 */
void Utf8::next(unsigned char c) {
  // insert character into union for analysis
  mUtf8Byte.ch = c;
  resume();

  // if necessary throw Match or Error exception
  switch (mDecodingStatus) {
    case ERROR:
      throw Error();
    case MATCH:
      throw Match(mBuiltCharCode);
    case DECODING:
      break;
  } // switch
}

/** Helper function for signaling a decoding error */
void Utf8::raiseError() {
  mDecodingStatus = ERROR;
  suspend();
  throw runtime_error("Resumed utf decoder after an error");
}

/** The body of the decoding functionality */
void Utf8::main() {
  int extraBytes; //! how many bytes to look for after the first byte
  unsigned int minUnicodeValue; //! the smallest allowed code for this range

  // determine how many bytes are left over based on the first byte
  if (mUtf8Byte.t1.ck == 0x0) {
    extraBytes = 0;
    mBuiltCharCode = mUtf8Byte.t1.dt;
    minUnicodeValue = 0x0;
  } else if (mUtf8Byte.t2.ck == 0x6) {
    extraBytes = 1;
    mBuiltCharCode = mUtf8Byte.t2.dt;
    minUnicodeValue = 0x80;
  } else if (mUtf8Byte.t3.ck == 0xe) {
    extraBytes = 2;
    mBuiltCharCode = mUtf8Byte.t3.dt;
    minUnicodeValue = 0x800;
  } else if (mUtf8Byte.t4.ck == 0x1e) {
    extraBytes = 3;
    mBuiltCharCode = mUtf8Byte.t4.dt;
    minUnicodeValue = 0x10000;
  } else {
    raiseError();
  } // if

  for (int i = 0; i < extraBytes; i++) {
    // wait for the next byte to be provided
    suspend();

    // verify that the new byte is valid
    if (mUtf8Byte.dt.ck != 0x2) {
      raiseError();
    } // if

    // amend the built value
    mBuiltCharCode = (mBuiltCharCode << 6) | mUtf8Byte.dt.dt;
  } // for

  if (mBuiltCharCode < minUnicodeValue) {
    // the value was not minimally encoded
    raiseError();
  }

  // I've finished building it if execution makes it here
  mDecodingStatus = MATCH;
  suspend();
}
