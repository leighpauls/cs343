#pragma once

#include "WATCard.h"
#include "Printer.h"
#include "Bank.h"

#include <deque>

using namespace std;

_Task WATCardOffice {
public:
  // Thrown if a transfer is lost
  _Event Lost {};

  WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers);
  WATCard::FWATCard create(unsigned int sid, unsigned int amount);
  WATCard::FWATCard transfer(
      unsigned int sid,
      unsigned int amount,
      WATCard *card);

  struct Job {
    unsigned int mStudentId;
    unsigned int mAmount;
    WATCard* mCard;

    WATCard::FWATCard mResult;
    Job(unsigned int sId, unsigned int amount, WATCard* card)
        : mStudentId(sId), mAmount(amount), mCard(card) {}
  };

  Job *requestWork();

private:
  enum States {
    CourierRendezvousDone = 'W',
    CreationRendezvousDone = 'C',
    TransferRendezvousDone = 'T',
  };

  _Task Courier {
   public:
    Courier(unsigned int id, WATCardOffice* office, Bank& bank);

   private:
    void main();
    unsigned int mId;
    WATCardOffice* mOffice;
    Bank& mBank;
    enum Status {
      StartTransfer = 't',
      DoneTransfer = 'T',
    };
  };

  void main();

  Printer& mPrinter;
  Bank& mBank;
  unsigned int mNumCouriers;
  Job* mPendingJob;
};
