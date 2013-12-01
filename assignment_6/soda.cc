#include "BottlingPlant.h"
#include "VendingMachine.h"
#include "NameServer.h"
#include "Student.h"
#include "Parent.h"
#include "Bank.h"
#include "WATCardOffice.h"
#include "ConfigParams.h"
#include "MPRNG.h"

#include <iostream>

using namespace std;

void usageQuit(const char* cmd) {
  cout<<"Usage: "<<cmd
      <<" [ config-file [ random-seed (> 0) ] ]"<<endl;
  exit(0);
}

int doConvert(const char* str, int minValue, const char* cmd) {
  char* end;
  int res = strtol(str, &end, 10);
  if ((const char*)end == str || res < minValue) {
    usageQuit(cmd);
  }
  return res;
}

void uMain::main() {
  unsigned int seed = getpid();
  const char* configFile = "soda.config";
  switch (argc) {
    case 3:
      seed = doConvert(argv[2], 0, argv[0]);
    case 2:
      configFile = argv[1];
    case 1:
      break;
    default:
      usageQuit(argv[0]);
  }
  ConfigParms conf;
  processConfigFile(configFile, conf);

  mprng.seed(seed);
  {
    Printer printer(
        conf.numStudents,
        conf.numVendingMachines,
        conf.numCouriers);
    Bank bank(conf.numStudents);
    Parent parent(printer, bank, conf.numStudents, conf.parentalDelay);
    NameServer nameServer(printer, conf.numVendingMachines, conf.numStudents);
    WATCardOffice cardOffice(printer, bank, conf.numCouriers);
    vector<VendingMachine*> machines;
    {
      BottlingPlant bottlingPlant(
          printer,
          nameServer,
          conf.numVendingMachines,
          conf.maxShippedPerFlavour,
          conf.maxStockPerFlavour,
          conf.timeBetweenShipments);

      vector<Student*> students;
      for (unsigned int i = 0; i < conf.numStudents; ++i) {
        students.push_back(new Student(
            printer,
            nameServer,
            cardOffice,
            i,
            conf.maxPurchases));
      }

      for (unsigned int i = 0; i < conf.numVendingMachines; ++i) {
        machines.push_back(new VendingMachine(
            printer,
            nameServer,
            i,
            conf.sodaCost,
            conf.maxStockPerFlavour));
      }

      // wait for the students to terminate first
      for (unsigned int i = 0; i < students.size(); ++i) {
        delete students[i];
      }
      // then wait for the plant+truck
    }
    // now it's safe for the machines to terminate
    for (unsigned int i = 0; i < machines.size(); ++i) {
      delete machines[i];
    }
    // wait for everything else to terminate
  }
  cout<<"***********************"<<endl;
}
