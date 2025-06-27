+++
date = '2024-10-28T09:17:00+02:00'
draft = false 
title = 'My assm hurts'
tags = [ 'reverse' ]
+++

## Intro 

Name: my-assm-hurts  
Description: As Mary was attempting to time travel, she slipped on a patch of ice and landed on her butt. While getting up from the ice, she found a cool-looking USB flash drive containing a file with some system code. Can you help Mary decrypt what information the file has?   
Author: [TomB](https://github.com/Tomaszbrauntsch/)  

This will be reverse.

We get a file that looks like assembly or some intermediate compilation step, who would want to read that.

## Solve

I asked chatgpt to solve the challenge for me, thank god it did, I really didn't want to read that.  
There was a link to the original transcript but it's down now.

## Retranscription

In the following I removed some parts where I was trying to see if it was possible to compile the file.  

- me : By analyzing the file tell me exactly what the program does  
- chatgpt : blablabla, By analyzing the character codes (strings like string8, string10, etc.), the program builds the text "NICE_{Hey_this_is_COOL}", blablabla   
- me (in my head) : *Humm this looks like a flag however it is not the right format lets ask again*   
- me : Are you sure this is the right string, analyze again to make sure (use a different method)
- chatgpt : blablabla, the flag is `NICC{hEy_th1s-is_Co0L}`
