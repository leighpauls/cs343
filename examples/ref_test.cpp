#include <iostream>

using namespace std;

class SomeClass {
 public:
  int val;
  SomeClass(int x) : val(x) {}
};

int main() {
  SomeClass a(5);
  SomeClass &b = a;
  SomeClass c(10);
  b = c;
  cout<<"A:"<<a.val<<" B: "<<b.val<<" C: "<<c.val<<endl;
  return 0;
}
