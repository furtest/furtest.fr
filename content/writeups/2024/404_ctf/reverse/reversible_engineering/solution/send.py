import socket

passwd_f = open("workdir/passwd.txt")
passwd = passwd_f.read()
passwd_f.close()
token_f = open("workdir/token.txt")
token = token_f.read()
token_f.close()

if len(passwd) != 16:
    print("Fail")
    quit()

print(f"Passwd : {passwd}")
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("challenges.404ctf.fr", 31999))
print(s.recv(4096).decode())
token += "\n"
s.send(token.encode())
print(s.recv(4096).decode())
passwd += "\n"
s.send(passwd.encode())
print(s.recv(4096).decode())
#nc challenges.404ctf.fr 31999
