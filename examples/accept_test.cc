
#include <iostream>

using namespace std;

_Monitor Accepter {
public:
  Accepter() : mI(0) {}
  void doAccept() {
    cout<<"Accepting"<<endl;
  }

  int mI;
  uCondition cond;
  void waitAll() {
    mI++;
    cout<<"about to try waiting: "<<mI<<endl;
    if (mI == 3) {
      while (!cond.empty()) {
        cout<<"signaling!"<<endl;
        cond.signal();
      }
    } else {
      cout<<"do wait..."<<endl;
      cond.wait();
    }
    cout<<"done waiting!: "<<mI<<endl;
  }
};

_Task AcceptingTask {
  uCondition cond;
public:
  void doAccept() {
    cout<<"about to wait"<<endl;
    cond.wait();
    cout<<"done waiting"<<endl;
  }
private:
  void main() {
    cout<<"task start"<<endl;
    while (1) {
      cout<<"Wait"<<endl;
      _Accept(doAccept);
      cond.signalBlock();
      cout<<"Accepted!"<<endl;
    }
  }
};

void uMain::main() {
  cout<<"Starting"<<endl;
  //uProcessor p1, p2;
  AcceptingTask t;
  cout<<"A"<<endl;
  t.doAccept();
  cout<<"B"<<endl;
  t.doAccept();
  cout<<"Done!"<<endl;
}
