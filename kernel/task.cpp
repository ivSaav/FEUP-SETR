#include "include/task.h"

void Task_StackInit(task_t* t) {
  *t->stackPointer = 0x11;
  t->stackPointer--;
  *t->stackPointer = 0x22;
  t->stackPointer--;
  *t->stackPointer = 0x33;
  t->stackPointer--;

  uint16_t usAddress = (uint16_t)t->func;
  *t->stackPointer = (uint16_t)(usAddress & (uint16_t)0x00ff);
  t->stackPointer--;

  usAddress >>= 8;
  *t->stackPointer = (uint16_t)(usAddress & (uint16_t)0x00ff);
  t->stackPointer--;

  *t->stackPointer = (StackType_t)0x00; /* R0 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x80;
  t->stackPointer--;

  /* Now the remaining registers.   The compiler expects R1 to be 0. */
  *t->stackPointer = (StackType_t)0x00; /* R1 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x02; /* R2 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x03; /* R3 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x04; /* R4 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x05; /* R5 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x06; /* R6 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x07; /* R7 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x08; /* R8 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x09; /* R9 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x10; /* R10 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x11; /* R11 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x12; /* R12 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x13; /* R13 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x14; /* R14 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x15; /* R15 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x16; /* R16 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x17; /* R17 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x18; /* R18 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x19; /* R19 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x20; /* R20 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x21; /* R21 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x22; /* R22 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x23; /* R23 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x24; /* R26 X */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x25; /* R26 X */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x26; /* R26 X */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x27; /* R27 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x28; /* R28 Y */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x29; /* R29 */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x30; /* R30 Z */
  t->stackPointer--;
  *t->stackPointer = (StackType_t)0x031; /* R31 */
  t->stackPointer--;
}
