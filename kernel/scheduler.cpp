#include "include/scheduler.h"

#include <Arduino.h>

#include "include/context.h"
#include "include/task.h"

task_t Tasks[NT];
volatile int cur_task = 0;
volatile task_t* volatile cur_TCB = &(Tasks[0]); /* Change in assembly
                                                     if name is changed */

int Sched_Init(void) {
  for (int x = 0; x < NT; x++) Tasks[x].func = 0;
}

int Sched_AddTask(void (*f)(void), int d, int p) {
  for (int x = 0; x < NT; x++)
    if (!Tasks[x].func) {
      Tasks[x].stackPointer = &(Tasks[x].stack[MAX_STACK_SIZE - 1]);
      Tasks[x].period = p;
      Tasks[x].delay = d;
      Tasks[x].exec = 0;
      Tasks[x].func = f;
      Task_StackInit(&Tasks[x]);

      return x;
    }
  return -1;
}

/* Called every tick */
void Sched_Schedule(void) {
  for (int x = 0; x < NT; x++) {
    if (Tasks[x].func) {
      if (Tasks[x].delay) {
        Tasks[x].delay--;
      } else {
        /* Schedule Task */
        Tasks[x].exec = 1;
        Tasks[x].delay = Tasks[x].period - 1;
      }
    }
  }
}

/* Called every tick */
void Sched_Dispatch(void) {
  for (int x = 0; x < NT; x++) {
    if ((Tasks[x].func) && (Tasks[x].exec)) {
      cur_task = x;
      cur_TCB = &(Tasks[cur_task]);  // Change in assembly if name is changed

      /* Delete task if one-shot */
      if (!Tasks[x].period) Tasks[x].func = 0;
      return;
    }
  }
}

void Sched_Start(void) {
  // Setup interrupts
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // OCR1A = 6250; // compare match register 16MHz/256/10Hz
  // OCR1A = 31250; // compare match register 16MHz/256/2Hz
  OCR1A = 31;               // compare match register 16MHz/256/2kHz
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