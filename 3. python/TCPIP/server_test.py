import time
import socketserver
from pymycobot.mycobot import MyCobot
import RPi.GPIO as GPIO


class TCPMessageHandler(socketserver.StreamRequestHandler):
        
    def handle(self):
        global mc
        """Process socket data

        """
        # process received data.
        try:
            data=bytes.decode(self.rfile.readline().strip())
        except Exception as ex:
                print(ex)
                return 
    #             socket_tcp.close()
    #             sys.exit(1)
        print(data)
        data = data.split(',')
        commend = []
        for dt in data:
            try:
                commend.append(int(dt))
            except ValueError:
                commend.append(None)
        print(commend)
        if len(commend)>0:
            print("Received:%s"%commend)
            if commend[0]==1:
#                 GPIO.output(21,GPIO.HIGH)
                self.wfile.write(str.encode(str(mc.get_angles())))
                print(mc.get_angles())
                print('GPIO.HIGH')
            elif commend[0]==0:
#                 GPIO.output(21,GPIO.LOW)
                pass
            try:
                self.wfile.write(str.encode(','.join([str(i) for i in commend])))
            except BrokenPipeError:
                pass
            time.sleep(0.05)


class SocketServer(object):


    def __init__(self, host, port):
        super().__init__()
        self.host = host
        self.port = port

    def start_server(self):
        while True:
            try:
                server = socketserver.TCPServer((self.host, self.port),
                                                TCPMessageHandler)
                break
            except:
                time.sleep(1)
                continue
        try:
            print(1)
            server.serve_forever()
        finally:
            print(2)
            server.server_close()


if __name__ == "__main__":
    host = '192.168.10.131'
    port = 8888
    server = SocketServer(host, port)
    try:
        mc = MyCobot('/dev/ttyAMA0', 1000000)
        print(mc)
    except:
        print("cont find cobot")
        exit(0)
    server.start_server()

