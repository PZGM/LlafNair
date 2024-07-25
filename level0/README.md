We debug the file with `gdb`.

```
gdb ./level0
```

Set a breakpoint at main and run the program:

```
(gdb) break main
(gdb) run
```

Step through the instructions with `nexti`.
```
(gdb) nexti
```
After stepping through a some instructions, we see that the program calls atoi function and crash with a Segmentation fault juat after calling `atoi`.

We restarted the program with an argument and found that it compares the return value from `atoi` to `0x1a7` (423). After this comparison, the program executes a new shell using the `execv` function:

```
0x08048ed9 <+25>:	cmp    eax,0x1a7
[...]
0x08048f51 <+145>:	call   0x8054640 <execv>
```
We ran the program with `423` as the argument:

```
./level0 423
```

This successfully dropped us into a shell as the level1 user, where we retrieved the flag:

```
cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```
