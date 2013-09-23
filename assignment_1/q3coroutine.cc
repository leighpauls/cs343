#include <iostream>
#include <fstream>
#include <stdexcept>
#include <iomanip>

using namespace std;

_Coroutine Utf8 {
public:
  struct Match {
    unsigned int unicode;
    Match( unsigned int unicode ) : unicode( unicode ) {}
  };
  struct Error {};
private:
  union UTF8 {
    unsigned char ch; //! character passed by cocaller

#if defined( _BIG_ENDIAN ) || BYTE_ORDER == BIG_ENDIAN
    // BIG ENDIAN architecture
    // Types for 1st utf-8 byte
    struct {
      unsigned char ck : 1;
      unsigned char dt : 7;
    } t1;
    struct {
      unsigned char ck : 3;
      unsigned char dt : 5;
    } t2;
    struct {
      unsigned char ck : 4;
      unsigned char dt : 4;
    } t3;
    struct {
      unsigned char ck : 5;
      unsigned char dt : 3;
    } t4;
    // Type for extra utf-8 bytes
    struct {
      unsigned char ck : 2;
      unsigned char dt : 6;
    } dt;
#else
    // LITTLE ENDIAN architecture
    // types for 1st utf-8 byte
    struct {
      unsigned char dt : 7;
      unsigned char ck : 1;
    } t1;
    struct {
      unsigned char dt : 5;
      unsigned char ck : 3;
    } t2;
    struct {
      unsigned char dt : 4;
      unsigned char ck : 4;
    } t3;
    struct {
      unsigned char dt : 3;
      unsigned char ck : 5;
    } t4;
    // type for extra utf-8 bytes
    struct {
      unsigned char dt : 6;
      unsigned char ck : 2;
    } dt;
#endif // if defined( _BIG_ENDIAN ) || BYTE_ORDER == BIG_ENDIAN
  } mUtf8Byte;

  unsigned int mBuiltCharCode; //! The resulting build code
  enum DecodingStatus {
    DECODING, //! More bytes are needed to decode
    ERROR,    //! The code is invalid, don't try to send more bytes
    MATCH,    //! The code is now valid, don't try to send more bytes
  } mDecodingStatus;

  /** Helper function for signaling a decoding error */
  void raiseError();
  /** The body of the decoding function */
  void main();
public:

  Utf8() : mDecodingStatus(DECODING) {}

  /**
   * Gives the decoder another byte to consume
   * @param c  The new byte to decode
   * @throws Error  when c makes the code invalid
   * @throws Match  when c completes a valid code
   */
  void next( unsigned char c ) {
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
};

void Utf8::raiseError() {
  mDecodingStatus = ERROR;
  suspend();
  throw runtime_error("Resumed utf decoder after an error");
}

void Utf8::main() {
  // determine how many bytes are left over based on the first byte
  int extraBytes; //! how many bytes to look for after the first byte
  unsigned int minUnicodeValue; //! the smallest allowed code for this range
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

/**
 * Convert from an array of bytes to an int
 * @param  bytes  char array to read from
 * @param  len    how many bytes to read
 * @return        unsinged int representation of bytes
 */
unsigned int fromByteArray(unsigned char *bytes, size_t len) {
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
void writeResultLine(
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
void utf8DecodeLine(unsigned char *buffer, size_t limit) {
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
void utf8DecodeStream(istream &in) {
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
void doUtf8Decode(int argc, char** argv) {
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
