+++
date = '2025-04-27T14:23:22+02:00'
draft = false 
title = 'iForensic'
tags = [ "forensic" ]
+++

iForensic is a serie of challenge where the goal is to study an iphone.  
Here is the global description of this serie :  

As you pass through customs, the customs officer asks you to hand over your phone and its unlock code. The phone is returned to you a few hours later...  
Suspicious, you send your phone to ANSSI's CERT-FR for analysis. CERT-FR analysts carry out a collection on the phone, consisting of a sysdiagnose and a backup.

We are given two tar archives, one containing a backup of the phone and the other what is apparently a crash report.  

# iCrash

Title: iCrash (intro)
Points: 25  
Number of solves: 661  
Description: It seems that a flag has hidden itself in the place where crashes are stored on the phone...  

We go to `private/var/mobile/Library/Logs/CrashReporter` and there is a file called `fcsc_intro.txt`.   
```
>>> cat fcsc_intro.txt
FCSC{7a1ca2d4f17d4e1aa8936f2e906f0be8}
```

# iDevice

Title: iDevice  
Points: 100  
Number of solves: 602  
Description:  
To start with, find some information of interest about the phone: iOS version and phone model identifier.  
The flag is in the format `FCSC{<model identifier>|<build number>}`. For example, for an iPhone 14 Pro Max running iOS 18.4 (22E240): `FCSC{iPhone15,3|22E240}`.  


To find these informations I opened the file named `Info.plist` at the root of the backup which contains informations about the device and found the information I was looking for.
```
	<key>Build Version</key>
	<string>20A362</string>
    ...
	<key>Product Type</key>
	<string>iPhone12,3</string>
```
So the flag is `FCSC{iPhone12,3|20A362}`.
