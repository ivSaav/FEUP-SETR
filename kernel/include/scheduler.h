#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>

#define NT 20
#define GLOBAL_STACK_SIZE 1500

int Sched_Init(void);

/* maxStackSize should be more than 40 */
int Sched_AddTask(void (*f)(void), int delay, int p, int deadline,
                  int maxStackSize, int isIdleTask);

void Sched_Schedule(void);

void Sched_Dispatch(void);

void Sched_Start(void);

#endif /* SCHEDULER_H */