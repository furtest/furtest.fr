+++
date = '2024-10-28T09:17:00+02:00'
draft = false 
title = 'What flag'
tags = [ 'reverse' ]
+++

## Intro

Name: what-flag  
Description: NICC recieved a mysterious email with an executable file that does nothing. Can you figure out what this executable does?  
Author: [TomB](https://github.com/Tomaszbrauntsch/)

This is reverse engineering.
The binary is not stripped, lets go with binary ninja.

## Solve

Main does nothing however we see a few functions named : u, h, h2, f, l, a, g
Looking inside of them we see what seems to be part of the flag.
Let's put these together
- u : `NI`
- h : `CC`
- h2 : `{`
- f : `uhH`
- l : `_fl@g`
- a : `_i`
- g : `_ThInk}`
We get : `NICC{uhH_fl@g_i_ThInk}`
