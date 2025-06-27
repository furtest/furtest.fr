+++
date = '2024-06-22T15:41:13+02:00'
draft = false 
title = 'Unbreakable'
tags = [ 'misc' ]
+++

The goal of this challenge is to bypass the following python script to print a file to the screen.   
We notice a blacklist which stops us from using quite a lot of characters.   
Using open to get the content of the file and printing it is no problem : `print(open('flag.txt','r').read())`   
The problem are the parenthesis in the eval which creates an error when trying to execute our command. `eval(ans + '()')`   
I first tried to use str and use one of the str methods to convert the None returned by print to a string and avoid throwing an exception.  
But it wasn't working so i found that it is possible to call multiple functions on one line by separating them with a comma.   
So the final payload is : `print(open('flag.txt','r').read()),print`


```python
#!/usr/bin/python3

banner1 = 'Coucou'

banner2 = 'EZ'

blacklist = [ ';', '"', 'os', '_', '\\', '/', '`',
              ' ', '-', '!', '[', ']', '*', 'import',
              'eval', 'banner', 'echo', 'cat', '%', 
              '&', '>', '<', '+', '1', '2', '3', '4',
              '5', '6', '7', '8', '9', '0', 'b', 's', 
              'lower', 'upper', 'system', '}', '{' ]

while True:
  ans = input('Break me, shake me!\n\n$ ').strip()
  
  if any(char in ans for char in blacklist):
    print(f'\n{banner1}\nNaughty naughty..\n')
  else:
    try:
      eval(ans + '()')
      print('WHAT WAS THAT?!\n')
    except:
      print(f"\n{banner2}\nI'm UNBREAKABLE!\n") 
```
