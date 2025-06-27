+++
date = '2025-04-28T12:00:00+02:00'
draft = false 
title = 'bashcrawl'
tags = ['misc']
+++

Bashcrawl is a series of challenges revolving around the [bashcrawl](https://github.com/mks22-dw/bashcrawl) game which purpose is to teach the basics of using a POSIX (Linux, BSD, UNIX) terminal.  
Having used Linux for a few years I did not need to learn how to use the command line and didn't have time either as I had a CTF to win.  
So I decided to use alternate ways of finding the answers to the challenges. This will mostly resolve around using `grep` to find where is the part related to the current challenge.   
If you want to try this yourself just clone the [repo](https://github.com/mks22-dw/bashcrawl).   
Without further ado lets get started.  

## Bashcrawl 1

Where are you ?  

This is an easy one and we simply need to give the name of the starting directory which is `entrance`.

## Bashcrawl 2

Where does the portal lead ?   

We are supposed to play the game until we encounter a portal (which is creating a link using `ln`) and give the name of the directory we end up in.   
But this is to long I don't have time to play this game so :
```bash
$ grep -r "portal"
```
```txt
cellar/armoury/chamber/spell:   echo "a portal that will allow you to walk through"
cellar/armoury/chamber/spell:   echo "ln -f -s `pwd`/../../../chapel/courtyard/aviary/hall portal"
.scrap:# ln -s ../../../../.rift portal
```

There are two places where a portal is mentionned in the .scrap file and in a file in the chamber . As a lazy person I of course start by the one I am already in, the portal that lead to the rift. Wrong flag, ok then it's the other one. I try the other one and it's correct, the flag is `spell`. 

## Bashcrawl 3-6

I am not going to give you all the solutions you will have to finish by yourself 😉


