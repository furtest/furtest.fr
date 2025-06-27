+++
date = '2024-06-22T15:41:13+02:00'
draft = false 
title = 'Stop drop and roll'
tags = [ 'misc' ]
+++

A simple misc challenge which involves using a TCP connection to play a simple game in which you have to answer to a request with specific words.

For exemple : `GORGE FIRE PHREAK` must be answered to by `STOP-ROLL-DROP`

To do so I wrote a simple python script :
```python
import socket
import time

def main():
    # Connect to the server
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("83.136.249.153", 30914))
    s.recv(4096).decode()
    s.sendall(b"y\n") # We must send 'y' to start the game
    message = s.recv(1024).decode()[19:] # We receive the data
    print(message)
    while True:
        response = create_response(handle_input(message)) # And we answer
        print(response)
        time.sleep(0.5)
        s.sendall(response.encode())
        time.sleep(0.5)
        message = s.recv(4096).decode() # And we start again
        print(message)


def create_response(input:list) -> str:
    back = str()
    for mon in input:
        # Add the correct word
        if mon == "GORGE":
            back += "STOP"
        elif mon == "FIRE":
            back += "ROLL"
        elif mon == "PHREAK":
            back += "DROP"
        back += "-"
    back = back[:-1] # Remove the last '-'
    back += "\n"
    return back

def handle_input(input:str) -> list:
    # Transform the data into a list to make it easier to handle
    data_raw = str()
    for c in input:
        data_raw += c
        if c == "\n": break
    data = list()
    word = str()
    for c in data_raw:
        if c != "," and c != " " and c != "\n":
            word += c
        elif c == ",":
            data.append(word)
            word = ""
    data.append(word)
    return data


if __name__ == "__main__":
    main()
```
