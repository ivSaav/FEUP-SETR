#ifndef TASK_H
#define TASK_H

#include "Arduino.h"

#define MAX_STACK_SIZE 50

typedef uint8_t StackType_t;

typedef struct {
  /* Must be first element in struct */
  volatile StackType_t* stackPointer;

  StackType_t stack[MAX_STACK_SIZE];

  /* period in ticks */
  int period;

  /* ticks until next activation */
  int delay;

  /* function pointer */
  void (*func)(void);

  /* activation counter */
  int exec;

} task_t;

void Task_StackInit(task_t* t);

#endif /* TASK_H */