#include "q2table.h"

/// helper function to check both forks
bool Table::philosopherCanPickUp(unsigned int philId) {
  unsigned int left = philId;
  unsigned int right = (philId + 1) % mForkStates.size();
  return !(mForkStates[left] || mForkStates[right]);
}

