#include "WATCardOffice.h"

WATCardOffice::WATCardOffice(
    Printer &prt,
    Bank &bank,
    unsigned int numCouriers)
    : mPrinter(prt),
      mBank(bank),
      mNumCouriers(numCouriers),
      mKilled(false) {}

FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
  WATCard* newCard = new WATCard();
  mPendingJob = new Job(sid, amount, newCard);
  mPrinter.print(Printer::WATCardOffice, CreationRendezvousDone, sid, amount);
  return newJob->mResult;
}

FWATCard WATCardOffice::transfer(
    unsigned int sid,
    unsigned int amount,
    WATCard* card) {
  mPendingJob = new Job(sid, amount, card);
  mPrinter.print(Printer::WATCardOffice, TransferRendezvousDone, sid, amount);
  return newJob->mResult;
}

Job* WATCardOffice::requestWork() {
  return mPendingJob;
}

Courier::Courier(unsigned int id, WATCardOffice* office, Bank& bank)
    : mId(id), mOffice(office), mBank(bank) {}

void WATCardOffice::Courier::main() {
  mOffice->mPrinter.print(Printer::Courier, mId, Printer::Starting);

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
        job->mId,
        job->mAmount);

    mBank.withdraw(job->mId, job->mAmount);

    if (mprng(1, 6) == 1) {
      // I lose the card
      job->mResult.exception(WATCardOffice::Lost());
      delete job->mCard;
      delete job;
    } else {
      // deliver the funds
      job->mResult.delivery(job->mCard);
      mOffice->mPrinter.print(
          Printer::Courier,
          mId,
          DoneTransfer,
          job->mId,
          job->mAmount);
      delete job;
    }
  }

  mOffice->mPrinter.print(Printer::Courier, mId, Printer::Finished);
}

void WATCardOffice::main() {
  mPrinter.print(Printer::WATCardOffice, Printer::Starting);

  // create the couriers
  vector<Courier*> couriers;
  for (int i = 0; i < mNumCouriers; ++i) {
    couriers.push_back(new Courier(this, mBank));
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

  // Wait for the couriers to quit
  mPendingWork = NULL;
  for (int i = 0; i < couriers.size(); ++i) {
    delete couriers[i];
  }

  mPrinter.print(Printer::WATCardOffice, Printer::Finished);
}
