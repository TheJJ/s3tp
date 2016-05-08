import socket

addr = "/tmp/test"
s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
s.bind(addr)
s.listen(1)

while True:
    c, a = s.accept()
    print("Connected: " + str(a))
    while True:
        data = c.recv(4096)
        if not data:
            break
        print(data)
