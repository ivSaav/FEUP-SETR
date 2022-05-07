#include <Arduino.h>

#define NT 20

int Sched_Init(void);

int Sched_AddT(void (*f)(void), int d, int p);

void Sched_Schedule(void);

void Sched_Dispatch(void);
