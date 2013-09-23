#pragma once

_Coroutine Utf8 {
public:
  struct Match {
    unsigned int unicode;
    Match(unsigned int unicode) : unicode( unicode ) {}
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

  void raiseError();
  void main();
public:

  Utf8() : mDecodingStatus(DECODING) {}
  void next(unsigned char c);
};
