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
            // you figure it out
        } t3;
        struct {
            // you figure it out
        } t4;
        struct {                                         // type for extra utf-8 bytes
            // you figure it out
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
            // you figure it out
        } t3;
        struct {
            // you figure it out
        } t4;
        struct {                                         // type for extra utf-8 bytes
            // you figure it out
        } dt;
#endif
    } utf8;
    // YOU MAY ADD PRIVATE MEMBERS
  public:
    // YOU MAY ADD CONSTRUCTOR/DESTRUCTOR IF NEEDED
    void next( unsigned char c ) {
        utf8.ch = c;                    // insert character into union for analysis
        resume();
        // if necessary throw Match or Error exception
    }
};
