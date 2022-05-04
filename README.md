# FEUP-SETR

## Setup VSCode development

1. Install Arduino IDE
2. Install Arduino extension
3. Select Programmer -> **AVRISP mkII**
4. Select Board -> **Arduino Uno**
5. Select Serial Port -> **COM3** ??

## Useful commands

```sh
# WSL / Linux
cd build
avr-objdump -d kernel.ino.elf > objdump.out
```
