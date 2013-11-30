#include "ConfigParams.h"

#include <iostream>
#include <fstream>
#include <limits>					// numeric_limits

using namespace std;


static bool comments( ifstream &in, string &name ) {
  for ( ;; ) {
    in >> name;
    if ( in.fail() ) return true;
    if ( name.substr(0,1) != "#" ) break;
    in.ignore( numeric_limits<int>::max(), '\n' ); // ignore remainder of line
  } // for
  return false;
} // comments

// Process the configuration file to set the parameters of the simulation.
void processConfigFile( const char *configFile, ConfigParms &cparms ) {
  const unsigned int Parmnum = 9;
  struct {
    const char *name;
    bool used;
    unsigned int *value;
  } static parms[Parmnum] = {
    { "SodaCost", false, &cparms.sodaCost },
    { "NumStudents", false, &cparms.numStudents },
    { "MaxPurchases", false, &cparms.maxPurchases },
    { "NumVendingMachines", false, &cparms.numVendingMachines },
    { "MaxStockPerFlavour", false, &cparms.maxStockPerFlavour },
    { "MaxShippedPerFlavour", false, &cparms.maxShippedPerFlavour },
    { "TimeBetweenShipments", false, &cparms.timeBetweenShipments },
    { "ParentalDelay", false, &cparms.parentalDelay },
    { "NumCouriers", false, &cparms.numCouriers },
  };
  string name;
  unsigned int value, cnt, posn;
  unsigned int numOfParm = 0;

  try {
    // open the configuration file for input
    ifstream in( configFile );

    // parameter names can appear in any order
    for ( cnt = 0 ; cnt < Parmnum; cnt += 1 ) {
      // eof ?
      if ( comments( in, name ) ) break;
      // linear search
      for ( posn = 0; posn < Parmnum && name != parms[posn].name; posn += 1 );
      // configuration not found ?
      if ( posn == Parmnum ) break;
      // duplicate configuration ?
      if ( parms[posn].used ) break;
      in >> value;
      if ( in.fail() ) break;
      // ignore remainder of line
      in.ignore( numeric_limits<int>::max(), '\n' );
      numOfParm += 1;
      parms[posn].used = true;
      *parms[posn].value = value;
    } // for

    if ( numOfParm != Parmnum ) {
      cerr << "Error: file \"" << configFile << "\" is corrupt." << endl;
      exit( EXIT_FAILURE );
    } // if
    if ( ! comments( in, name ) ) {			// ! eof ?
      cerr << "Error: file \"" << configFile << "\" has extraneous data." << endl;
      exit( EXIT_FAILURE );
    } // if
  } catch( uFile::Failure ) {
    cerr << "Error: could not open input file \"" << configFile << "\"" << endl;
    exit( EXIT_FAILURE );
  } // try
} // processConfigFile

