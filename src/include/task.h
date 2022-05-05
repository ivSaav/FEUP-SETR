#ifndef TASK_H
#define TASK_H

typedef struct {
  volatile uint8_t* stackPointer;
  uint8_t stack[50];

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