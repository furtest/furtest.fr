#!/bin/bash

rm -rf workdir 
mkdir workdir
cd workdir
mkdir ghtest

timeout 3s nc challenges.404ctf.fr 31998 > chall.zip
#if (( `file chall.zip` = 'chall.zip : empty' )); then
#	exit 1
#fi
unzip chall.zip
cd ..
/home/furtest/application/ghidra_10.4_PUBLIC/support/analyzeHeadless workdir/ghtest testghidra -import workdir/crackme.bin -postScript decompile.py
sed -i 's/byte/char/g' workdir/decompiled_encode.c
sed -i 's/void/char/g' workdir/decompiled_encode.c 
sed -i 's/FUN_0010123f/encode/g' workdir/decompiled_encode.c
#sed -i 's/long/char */g' workdir/decompiled_encode.c
python3 bruteforce_gen.py
gcc workdir/main.c workdir/decompiled_encode.c -o workdir/bruteforce
./workdir/bruteforce > workdir/passwd.txt
python3 send.py
#rm -rf 'workdir'
