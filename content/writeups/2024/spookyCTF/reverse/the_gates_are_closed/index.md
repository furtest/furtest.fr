+++
date = '2024-10-28T09:17:00+02:00'
draft = false 
title = 'The gates are closed'
tags = [ 'reverse' ]
+++

## Intro

Name: The gates are closed
Description: A USB drive was found in front of the locked gates of an abandoned cemetery. It may contain information regarding the strange sightings reported to nearby authorities in the graveyard, which NICC decided to investigate.
Author: [LoadinConfustion](https://github.com/loadinconfusion)

This will be a reverse engineering challenge as we are not provided a remote.

## Solve

I first execute the file and get : `Nothing is going on here... :D`
I then run `strings` on the file and get
```
_ITM_registerTMCloneTable
PTE1
u+UH
TklDQ3s0X1IzNGxfRmw0Z30=
Nothing is going on here... :D
;*3$"
GCC: (Debian 13.2.0-13) 13.2.0
Scrt1.o
```

We find a base64 encoded string, we decode it (`echo TklDQ3s0X1IzNGxfRmw0Z30= | base64 -d`) and get the flag.
`NICC{4_R34l_Fl4g}`
