#include <iostream>
#include <vector>

#include <uSemaphore.h>
#include <dirent.h>
#include <uC++.h>

#include "q1mapper.h"
#include "q1reducer.h"

using namespace std;

void printUsage(string execName) {
  cout<<"Usage: "<<execName
      <<" input-directory [ num-reducers ( > 0 ) [ queue-length ( > 0 ) "
      <<"[ sort-buffer-size ( >= 0 ) ] ] ]"<<endl;
}

void uMain::main() {
  int numReducers = 4;
  int queueLength = 16;
  int sortBufferSize = 0;
  switch (argc) {
    case 5:
      sortBufferSize = atoi(argv[4]);
    case 4:
      queueLength = atoi(argv[3]);
    case 3:
      numReducers = atoi(argv[2]);
    case 2:
      break;
    default:
      printUsage(argv[0]);
      return;
  }

  string inputDir(argv[1]);
  // verify the arguments
  if (numReducers <= 0 || queueLength <= 0 || sortBufferSize < 0) {
    printUsage(argv[0]);
    return;
  }

  DIR* dir = opendir(inputDir.c_str());
  if (dir == NULL) {
    cout<<"Error! Cannot open directory \""<<inputDir<<"\""<<endl;
    printUsage(argv[0]);
    return;
  }

  uSemaphore signal(0);

  // make the mapper tasks
  vector<Mapper*> mappers;
  struct dirent *curEnt;
  while ((curEnt = readdir(dir)) != NULL) {
    if (curEnt->d_type != DT_REG) {
      continue;
    }
    string filename = inputDir;
    filename += "/";
    filename += curEnt->d_name;
    osacquire(cout)<<"reading file: "<<filename<<endl;
    if (sortBufferSize == 0) {
      mappers.push_back(new Mapper(filename, queueLength, &signal));
    } else {
      // TODO
      *((int*)NULL) = 5;
    }
  }

  // make the reducer tasks
  vector<Reducer*> reducers;
  for (int i = 0; i < numReducers; ++i) {
    if (sortBufferSize == 0) {
      reducers.push_back(new Reducer(i, numReducers, &signal, mappers));
    } else {
      // TODO
      *((int*)NULL) = 5;
    }
  }

  // wait for all tasks to complete
  for (unsigned int i = 0; i < mappers.size(); ++i) {
    delete mappers[i];
  }
  for (unsigned int i = 0; i < reducers.size(); ++i) {
    delete reducers[i];
  }

  cout<<"Finished! Semaphore counter: "<<signal.counter()<<endl;
}
