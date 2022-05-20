#include "include/mutex.h"

#include "include/scheduler.h"

mutex_t Mutexes[NM];

void Mut_init() {
  for (int i = 0; i < NM; i++) Mutexes[i].isLocked = 0;
}

void lock(mutex_t* m, task_t* locker) {
  ENTER_CRITICAL();

  if (m->isLocked) {
    /* Task requesting lock will be blocked and inherit the priority of the
     * current holder */

    // Yield from task?

  } else {
    m->isLocked = 1;
    m->currentHolderDeadline = locker->deadline;
  }

  EXIT_CRITICAL();
}

void unlock(mutex_t* m, task_t* unlocker) {
  ENTER_CRITICAL();

  if (m->isLocked) {
    /* Should not happen */
  } else {
      m->isLocked = 0;
      m->currentHolderDeadline = 0;
  }

  EXIT_CRITICAL();
}
