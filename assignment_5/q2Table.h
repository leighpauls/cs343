
/// internal scheduling monitor solution
#if defined( TABLETYPE_INT )
_Monitor Table {
  // private declarations for this kind of table
  

/// internal scheduling monitor solution with barging
#elif defined( TABLETYPE_INTB )
_Monitor Table {
  /// only one condition variable
  uCondition waiting;
  /// barging version of wait
  void wait();


/// automatic-signal monitor solution
#elif defined( TABLETYPE_AUTO )
_Monitor Table {

/// internal/external scheduling task solution
#elif defined( TABLETYPE_TASK )
_Task Table {

#else
#error unsupported table
#endif

  // common declarations
public:                             // common interface
  Table( const unsigned int NoOfPhil, Printer &prt );
  void pickup( unsigned int id );
  void putdown( unsigned int id );
};
