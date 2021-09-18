# // An highlighted block
import traceback
import socket
import time
import sys
#RPi's IP
SERVER_IP = "192.168.10.144"       #输入正确的目标ip地址，请查看树莓派ip
SERVER_PORT = 8888

print("Starting socket: TCP...")
'''
00      reless all servo                                      示例：0,0,0,0,0,0,0,0,0,0
11      请求当前机械手坐标                                       示例：1,1,0,0,0,0,0,0,0,0
12      请求机械手移动绝对位置                                    示例：1,2,50,135.10,-45.10,90.10,-179.10,0.10,-90.10,0
13      请求机械手移动基于当前位置的相对偏移位置                     示例：1,3,50,20,0,0,0,0,0,0
21      请求当前机械手角度                                        示例：2,1,0,0,0,0,0,0,0,0
22      请求机械手移动绝对角度                                     示例：2,2,100,0,0,0,0,0,0,0
23      请求机械手移动基于当前角度的相对偏移角度                      示例：2,3,100,0,0,0,90,0,0,0
'''
print("Please input!")
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((SERVER_IP, SERVER_PORT))                        #打开链接
while True:
    try:
        # 
        # if len(data)>0:
        command=input()
        command +='\n'
        print("send: " + command)
        try:
            sock.sendall(str.encode(command))
            received = sock.recv(1024)
            print("read: "+ received.decode()) 
        except ConnectionRefusedError:
            print('error')
            sock.close()
            pass
        except BlockingIOError:
            pass
        except:
            pass
            
            
            
    except Exception:
        # print("exception")
        print(traceback.format_exc()) 
        sys.exit(1)

