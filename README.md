# FEUP-SETR

A multi-stack microkernel with full preemption, context switching,
EDF scheduling, and mutexes with priority inheritance protocol.

## Setup VSCode development

1. Install Arduino IDE
2. Install Arduino extension
3. Select Programmer -> **AVRISP mkII**
4. Select Board -> **Arduino Uno**
5. Select appropriate Serial Port

## Useful commands

```sh
# WSL / Linux
cd build
avr-objdump -d kernel.ino.elf > objdump.out
```
