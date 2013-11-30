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
  Job* newJob = new Job(sid, amount, newCard);
  mPendingJobs.push_back(newJob);
  mPrinter.print(Printer::WATCardOffice, CreationRendezvousDone, sid, amount);
  return newJob->mResult;
}

FWATCard WATCardOffice::transfer(
    unsigned int sid,
    unsigned int amount,
    WATCard* card) {
  Job* newJob = new Job(sid, amount, card);
  mPendingJobs.push_back(newJob);
  mPrinter.print(Printer::WATCardOffice, TransferRendezvousDone, sid, amount);
  return newJob->mResult;
}

Job* WATCardOffice::requestWork() {
  if (mKilled) {
    return NULL;
  }
  if (mPendingJobs.empty()) {
    // wait for the admin to give me work
    mWaitingForWork.wait();
    if (mKilled) {
      return NULL;
    }
  }
  mPrinter.print(Printer::WATCardOffice, CourierRendezvousDone);
  return mPendingJobs.pop_front();
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
    _Accept(transfer) {
      mWaitingForWork.signal();
    } or _Accept(~WATCardOffice) {
      break;
    }
  }

  // Tell the couriers to quit
  mKilled = true;
  while (!mWaitingForWork.empty()) {
    mWaitingForWork.signal();
  }
  // Wait for the couriers to quit
  for (int i = 0; i < couriers.size(); ++i) {
    delete couriers[i];
  }

  mPrinter.print(Printer::WATCardOffice, Printer::Finished);
}
