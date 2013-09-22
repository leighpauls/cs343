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
    unsigned char ch;                                // character passed by cocaller
#if defined( _BIG_ENDIAN ) || BYTE_ORDER == BIG_ENDIAN   // BIG ENDIAN architecture
    struct {                                         // types for 1st utf-8 byte
      unsigned char ck : 1;                        // check
      unsigned char dt : 7;                        // data
    } t1;
    struct {
      unsigned char ck : 3;                        // check
      unsigned char dt : 5;                        // data
    } t2;
    struct {
      unsigned char ck : 4;
      unsigned char dt : 4;
    } t3;
    struct {
      unsigned char ck : 5;
      unsigned char dt : 3;
    } t4;
    struct {                                         // type for extra utf-8 bytes
      unsigned char ck : 2;
      unsigned char dt : 6;
    } dt;
#else                                                    // LITTLE ENDIAN architecture
    struct {                                         // types for 1st utf-8 byte
      unsigned char dt : 7;                        // data
      unsigned char ck : 1;                        // check
    } t1;
    struct {
      unsigned char dt : 5;                        // data
      unsigned char ck : 3;                        // check
    } t2;
    struct {
      unsigned char dt : 4;                        // data
      unsigned char ck : 4;                        // check
    } t3;
    struct {
      unsigned char dt : 3;                        // data
      unsigned char ck : 5;                        // check
    } t4;
    struct {                                         // type for extra utf-8 bytes
      unsigned char dt : 6;
      unsigned char ck : 2;
    } dt;
#endif
  } m_utf8;

  void main();
  unsigned int m_built_char_code;
  enum DecodingStatus {
    DECODING,
    ERROR,
    MATCH,
  } m_decoding_status;
  
  void raise_error();

public:

  Utf8() : m_decoding_status(DECODING) {}

  // YOU MAY ADD CONSTRUCTOR/DESTRUCTOR IF NEEDED
  void next( unsigned char c ) {
    m_utf8.ch = c; // insert character into union for analysis
    resume();

    // if necessary throw Match or Error exception
    switch (m_decoding_status) {
      case ERROR:
        throw Error();
      case MATCH:
        throw Match(m_built_char_code);
      case DECODING:
        break;
    }
  }
};

void Utf8::raise_error() {
  m_decoding_status = ERROR;
  suspend();
  throw runtime_error("Resumed utf decoder after an error");
}

void Utf8::main() {
  int extra_bytes;
  // get a byte
  if (m_utf8.t1.ck == 0x0) {
    extra_bytes = 0;
    m_built_char_code = m_utf8.t1.dt;
  } else if (m_utf8.t2.ck == 0x6) {
    extra_bytes = 1;
    m_built_char_code = m_utf8.t2.dt;
  } else if (m_utf8.t3.ck == 0xe) {
    extra_bytes = 2;
    m_built_char_code = m_utf8.t3.dt;
  } else if (m_utf8.t4.ck == 0x1e) {
    extra_bytes = 3;
    m_built_char_code = m_utf8.t4.dt;
  } else {
    raise_error();
  }
  for (int i = 0; i < extra_bytes; i++) {
    // get a new byte
    suspend();
    // verify that the new byte is valid
    if (m_utf8.dt.ck != 0x2) {
      raise_error();
    }
    // amend the built value
    m_built_char_code = (m_built_char_code << 6) | m_utf8.dt.dt;
    // if m_built_char_code is still zero by this point, it must not be minially encoded
    if (m_built_char_code == 0) {
      raise_error();
    }
  }
  // I've finished building it if execution makes it here
  m_decoding_status = MATCH;
  suspend();
}

unsigned int from_byte_array(unsigned char *bytes, size_t len) {
  unsigned int res = 0;
  for (size_t i = 0; i < len; i++) {
    res = (res << 8) | bytes[i];
  }
  return res;
}

void write_result_line(unsigned char *buffer, size_t consumed, size_t input_len, bool success, unsigned int unicode) {
  // print the bytes consumed
  if (consumed > 0) {
    cout<<"0x"<<hex<<from_byte_array(buffer, consumed);
  }
  cout<<" : ";
  if (input_len == 0) {
    cout<<"Warning! Blank line."<<endl;
    return;
  }
  if (success) {
    cout<<"valid 0x"<<hex<<unicode;
  } else {
    cout<<"invalid";
  }
  
  if (consumed != input_len) {
    cout<<". Extra characters 0x"<<hex<<from_byte_array(&buffer[consumed], input_len - consumed);
  }
  cout<<endl;
}

void utf8_decode_line(unsigned char *buffer, size_t limit) {
  Utf8 decoder;
  int i;
  for (i = 0; i < limit; ++i) {
    try {
      decoder.next(buffer[i]);
    } catch (Utf8::Match match) {
      write_result_line(buffer, i+1, limit, true, match.unicode);
      return;
    } catch (Utf8::Error error) {
      write_result_line(buffer, i+1, limit, false, 0);
      return;
    }
  }
  // If the decoder gets here, then it has failed
  write_result_line(buffer, i, limit, false, 0);
}

void utf8_decode_stream(istream &in) {
  const size_t buffer_size = 32;
  unsigned char buffer[buffer_size];
  for (;;) {
    in.getline((char*)buffer, buffer_size);
    if (in.eof()) {
      // I've reached the end of the file safely
      return;
    }
    if (in.fail()) {
      throw runtime_error("Didn't allocate enough buffer space to read this line");
    }
    utf8_decode_line(buffer, in.gcount() - 1);
  }
}

void do_utf8_decode(int argc, char** argv) {
  if (argc <= 1) {
    cout<<"Reading from cin"<<endl;
    utf8_decode_stream(cin);
  } else {
    cout<<"Using file input: "<<argv[1]<<endl;
    ifstream input_file;
    input_file.open(argv[1], ios::in | ios::binary);

    if (input_file.is_open()) {
      utf8_decode_stream(input_file);
    } else {
      throw runtime_error("couldn't open the input file");
    }
  }
}

void uMain::main() {
  try {
    do_utf8_decode(argc, argv);
  } catch (runtime_error e) {
    cout<<"RUNTIME ERROR NOT CAUGHT: "<<e.what()<<endl;
    throw;
  }
}
