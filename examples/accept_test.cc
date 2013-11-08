
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
public:
  AcceptingTask(Accepter& a) : mA(a) {}
private:
  Accepter& mA;
  void main() {
    cout<<"Start waiting..."<<endl;
    mA.waitAll();
  }
};

void uMain::main() {
  cout<<"Starting"<<endl;
  //uProcessor p1, p2;
  Accepter a;
  AcceptingTask t1(a), t2(a);
  a.waitAll();
  cout<<"Done!"<<endl;
}
