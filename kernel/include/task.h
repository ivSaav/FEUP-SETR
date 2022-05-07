#ifndef TASK_H
#define TASK_H

#define STACK_SIZE 50

typedef struct {
  /* Must be first element in struct */
  volatile uint8_t* stackPointer;

  uint8_t stack[STACK_SIZE];

  /* period in ticks */
  int period;

  /* ticks until next activation */
  int delay;

  /* function pointer */
  void (*func)(void);

  /* activation counter */
  int exec;

} task_t;

#endif /* TASK_H */