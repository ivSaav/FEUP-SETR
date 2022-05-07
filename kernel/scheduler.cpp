#include "include/scheduler.h"

#include <Arduino.h>

#include "include/context.h"
#include "include/task.h"

volatile task_t Tasks[NT];
volatile int cur_task = 0;
volatile task_t* volatile cur_TCB =
    &(Tasks[0]);  // Change in assembly if name is changed

/* Hardware constants for timer 1 on ATMega323. */
#define portCLEAR_COUNTER_ON_MATCH (0x08)
#define portPRESCALE_256 (0x04)
#define portCLOCK_PRESCALER (256)
#define portCOMPARE_MATCH_A_INTERRUPT_ENABLE (0x10)

/*
 Setup timer 1 compare match A to generate a tick interrupt.
 */
static void prvSetupTimerInterrupt(void) {
  unsigned long ulCompareMatch;
  unsigned char ucHighByte, ucLowByte;

  /* Generate the compare match value for our required tick
  frequency. */
  // ulCompareMatch = portCPU_CLOCK_HZ / portTICK_RATE_HZ;
  ulCompareMatch = 31;

  /* We only have 16 bits so have to scale to get our
  required tick rate. */
  ulCompareMatch /= portCLOCK_PRESCALER;

  /* Setup compare match value for compare match A.
  Interrupts are disabled before calling this function so
  we need not bother here. [casting has been removed for
  each of reading] */
  ucLowByte = ulCompareMatch & 0xff;
  ulCompareMatch >>= 8;
  ucHighByte = ulCompareMatch & 0xff;
  OCR1AH = ucHighByte;
  OCR1AL = ucLowByte;

  /* Setup clock source and compare match behaviour. */
  ucLowByte = portCLEAR_COUNTER_ON_MATCH | portPRESCALE_256;
  TCCR1B = ucLowByte;

  /* Enable the interrupt - this is okay as interrupt
  are currently globally disabled. */
  ucLowByte = TIMSK1;
  ucLowByte |= portCOMPARE_MATCH_A_INTERRUPT_ENABLE;
  TIMSK1 = ucLowByte;
}

int Sched_Init(void) {
  for (int x = 0; x < NT; x++) Tasks[x].func = 0;
}

int Sched_AddTask(void (*f)(void), int d, int p) {
  for (int x = 0; x < NT; x++)
    if (!Tasks[x].func) {
      Tasks[x].stackPointer = &(Tasks[x].stack[STACK_SIZE - 1]);  // ????
      Tasks[x].period = p;
      Tasks[x].delay = d;
      Tasks[x].exec = 0;
      Tasks[x].func = f;

      *Tasks[x].stackPointer = 0x11;
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = 0x22;
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = 0x33;
      Tasks[x].stackPointer--;

      uint16_t usAddress = (uint16_t)Tasks[x].func;
      *Tasks[x].stackPointer = (uint16_t)(usAddress & (uint16_t)0x00ff);
      Tasks[x].stackPointer--;

      usAddress >>= 8;
      *Tasks[x].stackPointer = (uint16_t)(usAddress & (uint16_t)0x00ff);
      Tasks[x].stackPointer--;

      *Tasks[x].stackPointer = (uint8_t)0x00; /* R0 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x80;
      Tasks[x].stackPointer--;

      /* Now the remaining registers.   The compiler expects R1 to be 0. */
      *Tasks[x].stackPointer = (uint8_t)0x00; /* R1 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x02; /* R2 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x03; /* R3 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x04; /* R4 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x05; /* R5 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x06; /* R6 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x07; /* R7 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x08; /* R8 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x09; /* R9 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x10; /* R10 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x11; /* R11 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x12; /* R12 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x13; /* R13 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x14; /* R14 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x15; /* R15 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x16; /* R16 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x17; /* R17 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x18; /* R18 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x19; /* R19 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x20; /* R20 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x21; /* R21 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x22; /* R22 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x23; /* R23 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x24; /* R26 X */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x25; /* R26 X */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x26; /* R26 X */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x27; /* R27 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x28; /* R28 Y */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x29; /* R29 */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x30; /* R30 Z */
      Tasks[x].stackPointer--;
      *Tasks[x].stackPointer = (uint8_t)0x031; /* R31 */
      Tasks[x].stackPointer--;

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