#include <iostream>

using namespace std;

void uMain::main() {
  int len = 16;
  int* array = new int[len];
  int staticArray[16];
  cout<<"length = "<<(sizeof(array) / sizeof(int))<<", "
      <<(sizeof(staticArray) / sizeof(int))<<endl;
}

