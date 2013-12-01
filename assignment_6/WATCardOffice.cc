#include "WATCardOffice.h"
#include "MPRNG.h"

#include <iostream>
using namespace std;

WATCardOffice::WATCardOffice(
    Printer &prt,
    Bank &bank,
    unsigned int numCouriers)
    : mPrinter(prt),
      mBank(bank),
      mNumCouriers(numCouriers) {}

WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
  WATCard* newCard = new WATCard();
  mPendingJob = new Job(sid, amount, newCard);
  mPrinter.print(
      Printer::WATCardOffice,
      (char)CreationRendezvousDone,
      sid,
      amount);
  return mPendingJob->mResult;
}

WATCard::FWATCard WATCardOffice::transfer(
    unsigned int sid,
    unsigned int amount,
    WATCard* card) {
  mPendingJob = new Job(sid, amount, card);
  mPrinter.print(
      Printer::WATCardOffice,
      (char)TransferRendezvousDone,
      sid,
      amount);
  return mPendingJob->mResult;
}

WATCardOffice::Job* WATCardOffice::requestWork() {
  Job* res = mPendingJob;
  mPendingJob = NULL;
  return res;
}

WATCardOffice::Courier::Courier(
    unsigned int id,
    WATCardOffice* office,
    Bank& bank) : mId(id), mOffice(office), mBank(bank) {}

void WATCardOffice::Courier::main() {
  mOffice->mPrinter.print(Printer::Courier, mId, (char)Printer::Starting);

  for (;;) {
    Job* job = mOffice->requestWork();
    if (job == NULL) {
      // No more transfers, quit
      break;
    }

    // starting the transfer
    mOffice->mPrinter.print(
        Printer::Courier,
        mId,
        StartTransfer,
        job->mStudentId,
        job->mAmount);

    // apply the transfer
    mBank.withdraw(job->mStudentId, job->mAmount);
    job->mCard->deposit(job->mAmount);

    if (mprng(1, 6) == 1) {
      // I lose the card
      job->mResult.exception(new WATCardOffice::Lost());
      delete job->mCard;
    } else {
      // deliver the card
      job->mResult.delivery(job->mCard);
      mOffice->mPrinter.print(
          Printer::Courier,
          mId,
          DoneTransfer,
          job->mStudentId,
          job->mAmount);
    }
    delete job;
  }

  mOffice->mPrinter.print(Printer::Courier, mId, (char)Printer::Finished);
}

void WATCardOffice::main() {
  mPrinter.print(Printer::WATCardOffice, Printer::Starting);

  // create the couriers
  vector<Courier*> couriers;
  for (int i = 0; i < mNumCouriers; ++i) {
    couriers.push_back(new Courier(i, this, mBank));
  }

  for (;;) {
    // Wait for a creation or transfer, or to be killed
    _Accept(create, transfer) {
      // give the work to a courier
      _Accept(requestWork);
    } or _Accept(~WATCardOffice) {
      break;
    }
  }

  // tell all the couriers to quit
  mPendingJob = NULL;
  for (unsigned int i = 0; i < couriers.size(); ++i) {
    _Accept(requestWork);
  }

  // Wait for the couriers to quit
  for (unsigned int i = 0; i < couriers.size(); ++i) {
    delete couriers[i];
  }

  mPrinter.print(Printer::WATCardOffice, Printer::Finished);
}
