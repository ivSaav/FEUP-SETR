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
  Serial.println("Before init");
  for (int x = 0; x < NT; x++) {
    T_Objs[x].func = 0;
  }
}

int Sched_AddTask(void (*f)(void), int delay, int p, int deadline,
                  int maxStackSize, int isIdleTask) {
  for (int x = 0; x < NT; x++)
    
    if (!T_Objs[x].func) {
      // Tasks[x].stackPointer = &(Tasks[x].stack[MAX_STACK_SIZE - 1]);
      T_Objs[x].bottomOfStack = &(globalStack[curStackIndex]);
      T_Objs[x].stackPointer = &(globalStack[curStackIndex + maxStackSize - 1]);
      curStackIndex += maxStackSize;

      T_Objs[x].id = x;
      T_Objs[x].period = p;
      T_Objs[x].delay = delay;
      T_Objs[x].deadline = deadline;
      T_Objs[x].exec = 0;
      T_Objs[x].func = f;
      T_Objs[x].isIdleTask = isIdleTask;
      T_Objs[x].inheritedDeadline = deadline;

      Tasks[x] = &T_Objs[x];
      Task_StackInit(Tasks[x]);

      return x;
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
  const task_t *t1 = *((const task_t **) p1);
  const task_t *t2 = *((const task_t **) p2);

  // Non set tasks always go last
  if (!t1->func) return 1;
  if (!t2->func) return -1;

  if (t1->exec == t2->exec) {
    if (t1->isIdleTask) return 1;
    if (t2->isIdleTask) return -1;
    // return t1->deadline - t2->deadline;

    if (t1->inheritedDeadline == t2->inheritedDeadline) {
      return t1->blocked - t2->blocked;
    }
    return t1->inheritedDeadline - t2->inheritedDeadline;
  }

  return t2->exec - t1->exec;
}

// static void sortTasks() {
//   for (int i = 0; i < NT - 1; i++) {
//     for (int j = 0; j < NT - i - 1; j++) {
//       if (Task_cmp(&Tasks[j], &Tasks[j + 1]) > 0) {
//         // if (Tasks[j] > Tasks[j + 1]) {
//         task_t temp = Tasks[j];
//         Tasks[j] = Tasks[j + 1];
//         Tasks[j + 1] = temp;
//       }
//     }
//   }
// }

/* Called every tick */
void Sched_Dispatch(void) {
  Serial.println("BEFORE SCHED");
  qsort(Tasks, NT, sizeof(task_t*), Task_cmp);

  Serial.println("AFTER SCHED");

  cur_task = 0;
  cur_TCB = Tasks[cur_task];
  cur_TCB->blocked = 0;
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

  /* Restore the context of the first task that is going to run. */
  portRESTORE_CONTEXT();

  /* Simulate a function call end as generated by the compiler.  We will now
  jump to the start of the task the context of which we have just restored. */
  asm volatile("ret");
}