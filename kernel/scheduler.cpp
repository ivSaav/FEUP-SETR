#include "include/scheduler.h"

#include <Arduino.h>

#include "include/context.h"
#include "include/task.h"

task_t *Tasks[NT];
task_t T_Objs[NT];
volatile int cur_task = 0;
volatile task_t *volatile cur_TCB = Tasks[0]; /* Change in assembly
                                                     if name is changed */

StackType_t globalStack[GLOBAL_STACK_SIZE];
int curStackIndex = 0;

int Sched_Init(void) {
  for (int x = 0; x < NT; x++) {
    T_Objs[x].func = 0;
    Tasks[x] = &(T_Objs[x]);
  }
}

int Sched_AddTask(void (*f)(void), int delay, int p, int deadline,
                  int maxStackSize, int isIdleTask) {
  for (int x = 0; x < NT; x++) {
    if (!Tasks[x]->func) {
      Tasks[x]->bottomOfStack = &(globalStack[curStackIndex]);
      Tasks[x]->stackPointer = &(globalStack[curStackIndex + maxStackSize - 1]);
      curStackIndex += maxStackSize;

      Tasks[x]->id = x;
      Tasks[x]->period = p;
      Tasks[x]->delay = delay;
      Tasks[x]->deadline = deadline;
      Tasks[x]->exec = isIdleTask;
      Tasks[x]->func = f;
      Tasks[x]->isIdleTask = isIdleTask;
      Tasks[x]->inheritedDeadline = deadline;
      Tasks[x]->numRuns = 0;
      Task_StackInit(Tasks[x]);
      return x;
    }
  }
  return -1;
}

/* Called every tick */
int Sched_Schedule(void) {
  int switchContext = 0;
  for (int x = 0; x < NT; x++) {
    if (Tasks[x]->func) {
      if (Tasks[x]->delay) {
        Tasks[x]->delay--;
      } else {
        /* Schedule Task */
        Tasks[x]->exec = 1;
        Tasks[x]->delay = Tasks[x]->period - 1;
        switchContext = 1;
      }
    }
  }

  return switchContext;
}

static int Task_cmp(const void *p1, const void *p2) {
  const task_t *t1 = *((const task_t **)p1);
  const task_t *t2 = *((const task_t **)p2);

  // Non set tasks always go last
  if (!t1->func) return 1;
  if (!t2->func) return -1;

  if (t1->exec == t2->exec) {
    if (t1->isIdleTask) return 1;
    if (t2->isIdleTask) return -1;

    int prio1 = Task_GetPriority(t1);
    int prio2 = Task_GetPriority(t2);

    if (prio1 == prio2) {
      return t1->blocked - t2->blocked;
    }
    return prio1 - prio2;
  }

  return t2->exec - t1->exec;
}

/* Called every tick */
void Sched_Dispatch(void) {  
  qsort(Tasks, NT, sizeof(task_t *), Task_cmp);
  cur_task = 0;
  cur_TCB = Tasks[cur_task];
}

void Sched_Start(void) {
  // Setup interrupts
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // OCR1A = 6250; // compare match register 16MHz/256/10Hz
  OCR1A = 31250;  // compare match register 16MHz/256/2Hz
  // OCR1A = 31;               // compare match register 16MHz/256/2kHz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();

  // prvSetupTimerInterrupt();
  cur_TCB = Tasks[0];
  /* Restore the context of the first task that is going to run. */
  portRESTORE_CONTEXT();

  /* Simulate a function call end as generated by the compiler.  We will now
  jump to the start of the task the context of which we have just restored. */
  asm volatile("ret");
}