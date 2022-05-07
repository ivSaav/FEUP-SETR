#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>

#define NT 20

int Sched_Init(void);

int Sched_AddTask(void (*f)(void), int d, int p);

void Sched_Schedule(void);

void Sched_Dispatch(void);

void Sched_Start(void);

#endif /* SCHEDULER_H */