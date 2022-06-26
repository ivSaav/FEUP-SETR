#ifndef TASK_H
#define TASK_H

#include "Arduino.h"

#define MAX_STACK_SIZE 50

typedef uint8_t StackType_t;

typedef struct task_t {
  /* Must be first element in struct */
  volatile StackType_t* stackPointer;

  StackType_t* bottomOfStack;

  // StackType_t stack[MAX_STACK_SIZE];

  /* period in ticks */
  int period;

  /* ticks until next activation */
  int delay;

  /* deadline in ticks */
  int deadline;

  /* function pointer */
  void (*func)(void);

  /* activation counter */
  int exec;

  int isIdleTask;

  /* Used for Priority Inheritance */
  int inheritedDeadline;

  /* Used for task identification */
  int id;

  int blocked;

  int numRuns;

  int initialDelay;

  volatile task_t* inherited;
} task_t;

void Task_StackInit(task_t* t);

int Task_GetPriority(const task_t* t);

#endif /* TASK_H */