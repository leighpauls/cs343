#include "Student.h"
#include "VendingMachine.h"
#include "MPRNG.h"

Student::Student(
    Printer &prt,
    NameServer &nameServer,
    WATCardOffice &cardOffice,
    unsigned int id,
    unsigned int maxPurchases)
    : mPrinter(prt),
      mNameServer(nameServer),
      mCardOffice(cardOffice),
      mId(id),
      mMaxPurchases(maxPurchases) {}

void Student::main() {
  unsigned int bottlesToBuy = mprng(1, mMaxPurchases);
  VendingMachine::Flavours favoriteFlavour = (VendingMachine::Flavours)mprng(
      (unsigned int)VendingMachine::NUM_FLAVOURS);
  mPrinter.print(
      Printer::Student,
      mId,
      Printer::Starting,
      favoriteFlavour,
      bottlesToBuy);

  mCardOffice.create(mId, 5);
  VendingMachine* machine = NULL;
  WATCard::FWATCard cardFuture = mCardOffice.create(mId, 5);
  while (bottlesToBuy > 0) {
    if (machine == NULL) {
      // Need a new machine
      machine = mNameServer.getMachine(mId);
      mPrinter.print(
          Printer::Student,
          mId,
          (char)SelectMachine,
          machine->getId());
    }

    // wait a little
    for (int i = mprng(1, 10); i > 0; i--) { yield(); }
    try {
      // make sure I've got a card
      WATCard* card = cardFuture();
      // buy some soda
      VendingMachine::Status status = machine->buy(
          favoriteFlavour,
          *(cardFuture()));
      switch (status) {
        case VendingMachine::BUY:
          bottlesToBuy--;
          mPrinter.print(
              Printer::Student,
              mId,
              (char)BoughtSoda,
              card->getBalance());
          break;
        case VendingMachine::STOCK:
          // find a new machine
          machine = NULL;
          break;
        case VendingMachine::FUNDS:
          cardFuture = mCardOffice.transfer(mId, machine->cost() + 5, card);
          break;
      }
    } catch (WATCardOffice::Lost l) {
      // ask for a new card
      cardFuture = mCardOffice.create(mId, 5);
      mPrinter.print(Printer::Student, mId, (char)WATCardLost);
    }
  }
  mPrinter.print(Printer::Student, mId, (char)Printer::Finished);
}
