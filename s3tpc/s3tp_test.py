import socket
from struct import pack


class S3TPTester:
    def __init__(self, addr):
        self.s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.s.bind(addr)

    def start(self):
        self.s.listen(1)

        while True:
            self.c, a = self.s.accept()
            print("New Connection")
            try:
                self.handle_connection()
            except Exception as e:
                print(str(e))

    def handle_connection(self):
        while True:
            self.communicate(1, pack("<H", 1337))
            self.communicate(1, pack("<H", 235))
            self.communicate(4)
            self.communicate(4)

    def communicate(self, opcode, content=b""):
            self.receive()
            self.respond(opcode, content)

    def receive(self):
        data = self.c.recv(4096)
        if not data:
            raise Exception("Closed connection")
        print("Received: {}".format(data))
        self.opcode = data[0:2]
        self.event_id = data[2:6]
        self.content = data[6:]


    def respond(self, opcode, content=b""):
        response = pack("<H", opcode) + self.event_id + content
        print("Sending response: {}".format(response))
        self.c.sendall(response)


if __name__ == '__main__':
    tester = S3TPTester("/tmp/test")
    tester.start()
