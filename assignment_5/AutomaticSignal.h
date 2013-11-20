
#define AUTOMATIC_SIGNAL \
  uCondition __mAutoCondition;

#define WAITUNTIL( pred, before, after )        \
  {                                             \
    if (!(pred)) {                              \
      before;                                   \
      do {                                      \
        __mAutoCondition.signalBlock();         \
        __mAutoCondition.wait();                \
      } while (!(pred));                        \
      after;                                    \
    }                                           \
  }

#define RETURN( expr... )                       \
  {                                             \
    __mAutoCondition.signalBlock();             \
    return expr;                                \
  }
