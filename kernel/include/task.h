#ifndef TASK_H
#define TASK_H

#define MAX_STACK_SIZE 50

#include "Arduino.h"

typedef uint8_t StackType_t;

class Task {
 public:
  Task(void (*f)(void), int d, int p);
  Task();

  void initStack();

 public:
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
};

// typedef struct {
//   /* Must be first element in struct */
//   volatile uint8_t* stackPointer;

//   uint8_t stack[STACK_SIZE];

//   /* period in ticks */
//   int period;

//   /* ticks until next activation */
//   int delay;

//   /* function pointer */
//   void (*func)(void);

//   /* activation counter */
//   int exec;

// } task_t;

#endif /* TASK_H */