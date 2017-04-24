set $lastcs = -1

echo + target remote localhost:26000\n
target remote localhost:26000

echo + symbol-file kernel7.elf\n
symbol-file ./kernel7.elf
