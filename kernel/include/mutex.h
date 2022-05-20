#ifndef MUTEX_H
#define MUTEX_H

#include "task.h"

#define NM 5

#define ENTER_CRITICAL()                                \
  asm volatile("in		__tmp_reg__, __SREG__" ::); \
  asm volatile("cli" ::);                                   \
  asm volatile("push	__tmp_reg__" ::);

#define EXIT_CRITICAL()                       \
  asm volatile("pop		__tmp_reg__" ::); \
  asm volatile("out		__SREG__, __tmp_reg__" ::);

typedef struct {
  /* Deadline of the task currently holding the mutex */
  int currentHolderDeadline;

  int isLocked = 0;

} mutex_t;

void Mut_init();

void lock(mutex_t* m, task_t* locker);

void unlock(mutex_t* m);

#endif /* MUTEX_H */