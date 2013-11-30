#include "Student.h"
#include "VendingMachine.h"

Student::Student(
    Printer &prt,
    NameServer &nameServer,
    WATCardOffice &cardOffice,
    unsigned int id,
    unsigned int maxPurchases)
    : mPrinter(prt),
      mNameServer(nameServer),
      mCardOffive(cardOffice),
      mId(id),
      mMaxPurchases(maxPurchases) {}

void Student::main() {
  unsigned int bottlesToBuy = mprng(1, mMaxPurchases);
  VedingMachine::Flavours favoriteFlavour = mprng(
      VendingMachine::Flavours::NUM_FLAVOURS);
  mPrinter.print(
      Printer::Student,
      mId,
      Printer::Starting,
      favoriteFlavour,
      bottlesToBuy);

  mCardOffice.create(mId, 5);
  VendingMachine* machine = NULL;
  FWatCard cardFuture = mCardOffice.create(mId, 5);
  while (bottlesToBuy > 0) {
    if (machine == NULL) {
      // Need a new machine
      machine = mNameServer.getMachine(mId);
      mPrinter.print(Printer::Student, mId, SelectMachine, machine.getId());
    }

    // wait a little
    for (int i = mprng(1, 10); i > 0; i--) { yield(); }
    try {
      // make sure I've got a card
      WATCard* card = cardFuture();
      // buy some soda
      VendingMachine::Status status = machne->buy(
          favoriteFlavour,
          *(cardFuture()));
      switch (status) {
        case BUY:
          bottlesToBuy--;
          mPrinter.print(Printer::Student, mId, BoughtSoda, card->getBalance());
          break;
        case STOCK:
          // find a new machine
          machine = NULL;
          break;
        case FUNDS:
          cardFuture = mCardOffice.transfer(mId, machine->cost() + 5, card);
          break;
      }
    } catch (Lost l) {
      // ask for a new card
      cardFuture = mCardOffice.create(mId, 5);
      mPrinter.print(Printer::Student, mId, WATCardList);
    }
  }
  mPrinter.print(Printer::Student, mId, Printer::Finished);
}
