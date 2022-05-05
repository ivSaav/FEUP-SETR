#include <Arduino.h>

#include "include/context.h"
#include "include/scheduler.h"
#include "include/task.h"

extern volatile task_t Tasks[NT];
extern volatile int cur_task;

#define d1 13
#define d2 12
#define d3 11
#define d4 10

/* 4 Tasks:
 *     T1 - T = 100ms   -> Led d1 toggle
 *     T2 - T = 200ms   -> Led d2 toggle
 *     T3 - T = 600ms   -> Led d3 toggle
 *     T4 - T = 100ms   -> Led d4 copied from button A1
 */

void toggle(void) { digitalWrite(d4, !digitalRead(d4)); }

// --------------------------- Task Definition --------------------------
void t0(void) {
  static int A1_old;

  while (1) {
    int A1_new = digitalRead(A4);

    if (A1_new != A1_old) Sched_AddT(toggle, 2000 /* delay */, 0 /* period */);

    A1_old = A1_new;
  }
}

void t1(void) {
  digitalWrite(d1, !digitalRead(d1));
  delay(700);
}
void t2(void) { digitalWrite(d2, !digitalRead(d2)); }
void t3(void) {
  while (1) {
    digitalWrite(d3, !digitalRead(d3));
    delay(10);
  }
}
void t4(void) {
  while (1) {
    digitalWrite(d4, !digitalRead(d4));
    delay(10);
  }
}

// -----------------------------------------------------------------------

// ISR(TIMER1_COMPA_vect) {  // timer1 interrupt
//   Sched_Schedule();
//   Sched_Dispatch();
// }

// Verficar se é este o interrupt que queremos capturar?
// void TIMER1_COMPA_vect(void) __attribute__((signal, naked));

void vTaskIncrementTick(void) __attribute__((naked));
void vTaskIncrementTick(void) {
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

void vTaskSwitchContext(void) __attribute__((naked));
void vTaskSwitchContext(void) {
  // Serial.println("Entered Switch Context");
  // Serial.println("Current Task: " + cur_task);
  int prev_task = cur_task;
  for (int x = 0; x < cur_task; x++) {
    if ((Tasks[x].func) && (Tasks[x].exec)) {
      Tasks[x].exec = 0;
      cur_task = x;
      // interrupts();
      // Tasks[x].func();
      // noInterrupts();
      // cur_task = prev_task;
      /* Delete task if one-shot */
      if (!Tasks[x].period) Tasks[x].func = 0;
    }
  }
}

void vPortYieldFromTick(void) __attribute__((naked));
void vPortYieldFromTick(void) {
  portSAVE_CONTEXT();

  vTaskIncrementTick();
  vTaskSwitchContext();

  portRESTORE_CONTEXT();
  asm volatile("ret");
}

/* Interrupt service routine for the OS tick. */
ISR(TIMER1_COMPA_vect, ISR_NAKED) {
  Serial.println("A");

  vPortYieldFromTick();

  Serial.println("B");
  asm volatile("reti");
}

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(d4, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d1, OUTPUT);

  Sched_Init();

  Sched_AddT(t4, 1 /* delay */, 10 /* period */);
  Sched_AddT(t3, 1 /* delay */, 200 /* period */);
  // Sched_AddT(t2, 1 /* delay */, 500 /* period */);

  // /* add a bunch of one-shot tasks, that will temporarily take up space in
  // the
  //  * TCB array */
  // /* This forces task t1 to have a lower priority, and leave empty TCB
  // entries
  //  * for the toggle task added by t0.
  //  */
  // Sched_AddT(toggle, 10 /* delay */, 0 /* period */);
  // Sched_AddT(toggle, 20 /* delay */, 0 /* period */);
  // Sched_AddT(toggle, 30 /* delay */, 0 /* period */);
  // Sched_AddT(toggle, 40 /* delay */, 0 /* period */);
  // Sched_AddT(toggle, 50 /* delay */, 0 /* period */);
  // Sched_AddT(toggle, 60 /* delay */, 0 /* period */);
  // Sched_AddT(toggle, 70 /* delay */, 0 /* period */);
  // Sched_AddT(toggle, 80 /* delay */, 0 /* period */);

  // Sched_AddT(t1, 1 /* delay */, 1000 /* period */);
}

// the loop function runs over and over again forever
void loop() { /* nothing to do */
}
