#include "include/mutex.h"

#include "include/context.h"
#include "include/scheduler.h"

mutex_t Mutexes[NM];
extern task_t Tasks[NT];

/* Mutex yield */
void MutexYield(void) __attribute__((naked));
void MutexYield(void) {
  portSAVE_CONTEXT();

  // Serial.println(F("Task Yielded"));
  // Serial.println(Tasks[cur_task].deadline);

  Sched_Dispatch();

  portRESTORE_CONTEXT();

  asm volatile("ret");
}

void Mut_init() {
  for (int i = 0; i < NM; i++) Mutexes[i].isLocked = 0;
}

void lock(mutex_t* m, volatile task_t* locker) {
  ENTER_CRITICAL();

  int locked = 0;
  while (!locked) {
    if (m->isLocked) {
      /* Task requesting lock will be blocked and inherit the priority of the
       * current holder */

      // Yield from task?

      task_t* holder;
      for (int i = 0; i < NT; i++) {
        if (Tasks[i].id == m->holderId) {
          holder = &Tasks[i];
          break;
        }
      }

      holder->inheritedDeadline = locker->inheritedDeadline;
      locker->blocked = 1;

    } else {
      m->isLocked = 1;
      m->currentHolderDeadline = locker->deadline;
      m->holderId = locker->id;
      locked = 1;
      Serial.println(F("locked"));
    }
    MutexYield();
  }
  EXIT_CRITICAL();
}

void unlock(mutex_t* m, volatile task_t* unlocker) {
  ENTER_CRITICAL();

  if (m->isLocked) {
    /* Should not happen */
    m->isLocked = 0;
    m->currentHolderDeadline = 0;
    m->holderId = -1;

    task_t* holder;
    for (int i = 0; i < NT; i++) {
      if (Tasks[i].id == m->holderId) {
        holder = &Tasks[i];
        break;
      }
    }

    unlocker->inheritedDeadline = unlocker->deadline;
  } else {
  }

  Serial.println(F("Unlocked"));
  EXIT_CRITICAL();
  MutexYield();
}
