#pragma once

/**
 * A Co-routine base class that catches runtime_error's and prints them
 */
_Coroutine Catching {
private:
  void main();
protected:
  virtual void doMain() = 0;
  virtual ~Catching() {};
};
