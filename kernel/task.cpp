#include "include/task.h"

Task::Task() {
  this->stackPointer = &(this->stack[MAX_STACK_SIZE - 1]);
  this->period = 0;
  this->delay = 0;
  this->exec = 0;
  this->func = 0;
}

Task::Task(void (*f)(void), int d, int p) {
  this->stackPointer = &(this->stack[MAX_STACK_SIZE - 1]);
  this->period = p;
  this->delay = d;
  this->exec = 0;
  this->func = f;

  this->initStack();
}

void Task::initStack() {
  /* Known values */
  *(this->stackPointer) = 0x11;
  this->stackPointer--;
  *(this->stackPointer) = 0x22;
  this->stackPointer--;
  *(this->stackPointer) = 0x33;
  this->stackPointer--;

  // Setting the PC to the task function
  uint16_t usAddress = (uint16_t)this->func;
  *(this->stackPointer) = (uint16_t)(usAddress & (uint16_t)0x00ff);
  this->stackPointer--;

  usAddress >>= 8;
  *(this->stackPointer) = (uint16_t)(usAddress & (uint16_t)0x00ff);
  this->stackPointer--;

  *(this->stackPointer) = (StackType_t)0x00; /* R0 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x80; /* Disable interrupts */
  this->stackPointer--;

  /* Now the remaining registers.   The compiler expects R1 to be 0. */
  *(this->stackPointer) = (StackType_t)0x00; /* R1 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x02; /* R2 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x03; /* R3 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x04; /* R4 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x05; /* R5 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x06; /* R6 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x07; /* R7 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x08; /* R8 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x09; /* R9 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x10; /* R10 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x11; /* R11 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x12; /* R12 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x13; /* R13 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x14; /* R14 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x15; /* R15 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x16; /* R16 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x17; /* R17 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x18; /* R18 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x19; /* R19 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x20; /* R20 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x21; /* R21 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x22; /* R22 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x23; /* R23 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x24; /* R26 X */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x25; /* R26 X */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x26; /* R26 X */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x27; /* R27 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x28; /* R28 Y */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x29; /* R29 */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x30; /* R30 Z */
  this->stackPointer--;
  *(this->stackPointer) = (StackType_t)0x031; /* R31 */
  this->stackPointer--;
}
