#ifndef TASK_H
#define TASK_H



typedef struct {
  /* period in ticks */
  int period;
  /* ticks until next activation */
  int delay;
  /* function pointer */
  void (*func)(void);
  /* activation counter */
  int exec;

  uint16_t* stackPointer;
  uint8_t stack[10];
} task_t;

#endif /* TASK_H */