#include <Arduino.h>

#include "include/context.h"
#include "include/scheduler.h"
#include "include/task.h"

extern Task Tasks[NT];
extern volatile int cur_task;
extern volatile StackType_t* volatile cur_TCB; /*Change in assembly
                                                  if name is changed */

#define d1 13
#define d2 12
#define d3 11
#define d4 10

/* Tick incrementation and switching of context if needed */
void vPortYieldFromTick(void) __attribute__((naked));
void vPortYieldFromTick(void) {
  portSAVE_CONTEXT();

  Sched_Schedule(); /* vTaskIncrementTick in FreeRTOS */
  Sched_Dispatch(); /* vTaskSwitchContext in FreeRTOS */

  portRESTORE_CONTEXT();
  asm volatile("ret");
}

/* Manual yield */
void vPortYield(void) __attribute__((naked));
void vPortYield(void) {
  portSAVE_CONTEXT();

  Sched_Dispatch(); /* vTaskSwitchContext in FreeRTOS */

  portRESTORE_CONTEXT();

  asm volatile("ret");
}

/* Interrupt service routine for the OS tick. */
ISR(TIMER1_COMPA_vect, ISR_NAKED) {
  vPortYieldFromTick();

  asm volatile("reti");
}

void t3(void) {
  while (1) {
    digitalWrite(d3, !digitalRead(d3));
    Tasks[cur_task].exec = 0;
    vPortYield();
  }
}

void t4(void) {
  while (1) {
    digitalWrite(d4, !digitalRead(d4));
    Tasks[cur_task].exec = 0;
    vPortYield();
  }
}

void idle(void) {
  while (1) {
    // Serial.println("Idle");
    digitalWrite(d1, !digitalRead(d1));
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  // Serial.begin(115200);
  // while (!Serial) {
  //   ;
  // }

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(d4, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d1, OUTPUT);
  digitalWrite(d4, !digitalRead(d4));
  digitalWrite(d3, !digitalRead(d3));
  digitalWrite(d2, !digitalRead(d2));
  digitalWrite(d1, !digitalRead(d1));

  Sched_Init();

  Sched_AddTask(t3, 1 /* delay */, 1000 /* period */);
  Sched_AddTask(t4, 1 /* delay */, 500 /* period */);
  Sched_AddTask(idle, 1 /* delay */, 1 /* period */);

  // Serial.println(Tasks[0].period);
  Sched_Start();
}

// the loop function runs over and over again forever
void loop() { /* nothing to do */
}
