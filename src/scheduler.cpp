#include "include/scheduler.h"

#include <Arduino.h>

#include "include/task.h"

task_t Tasks[NT];
int cur_task = NT;

int Sched_Init(void) {
  for (int x = 0; x < NT; x++) Tasks[x].func = 0;
  /* Also configures interrupt that periodically calls Sched_Schedule(). */
  noInterrupts();  // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // OCR1A = 6250; // compare match register 16MHz/256/10Hz
  // OCR1A = 31250; // compare match register 16MHz/256/2Hz
  OCR1A = 31;               // compare match register 16MHz/256/2kHz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

int Sched_AddT(void (*f)(void), int d, int p) {
  for (int x = 0; x < NT; x++)
    if (!Tasks[x].func) {
      Tasks[x].period = p;
      Tasks[x].delay = d;
      Tasks[x].exec = 0;
      Tasks[x].func = f;
      return x;
    }
  return -1;
}

void Sched_Schedule(void) {
  for (int x = 0; x < NT; x++) {
    if (Tasks[x].func) {
      if (Tasks[x].delay) {
        Tasks[x].delay--;
      } else {
        /* Schedule Task */
        Tasks[x].exec++;
        Tasks[x].delay = Tasks[x].period - 1;
      }
    }
  }
}

void Sched_Dispatch(void) {
  int prev_task = cur_task;
  for (int x = 0; x < cur_task; x++) {
    if ((Tasks[x].func) && (Tasks[x].exec)) {
      Tasks[x].exec = 0;
      cur_task = x;
      interrupts();
      Tasks[x].func();
      noInterrupts();
      cur_task = prev_task;
      /* Delete task if one-shot */
      if (!Tasks[x].period) Tasks[x].func = 0;
    }
  }
}