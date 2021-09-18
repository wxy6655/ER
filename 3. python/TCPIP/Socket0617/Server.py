import time
import socketserver
from pymycobot.mycobot import MyCobot
import numpy as np



'''
00      reless all servo                                      示例：0,0,0,0,0,0,0,0,0,0
11      请求当前机械手坐标                                       示例：1,1,0,0,0,0,0,0,0,0
12      请求机械手移动绝对位置                                    示例：1,2,50,135.10,-45.10,90.10,-179.10,0.10,-90.10,0
13      请求机械手移动基于当前位置的相对偏移位置                     示例：1,3,50,20,0,0,0,0,0,0
21      请求当前机械手角度                                        示例：2,1,0,0,0,0,0,0,0,0
22      请求机械手移动绝对角度                                     示例：2,2,100,0,0,0,0,0,0,0
23      请求机械手移动基于当前角度的相对偏移角度                      示例：2,3,100,0,0,0,90,0,0,0
'''



class TCPMessageHandler(socketserver.StreamRequestHandler):
        
    def handle(self):
        global mc, run_mode
        """Process socket data

        """
        # process received data.
        while True:
            try:
                # data=bytes.decode(self.rfile.readline().strip())
                data= bytes.decode(self.request.recv(1024))
                print(data)
            except Exception as ex:
                    print(ex)
                    return 

            data = data.split(',')
            commend = []

            for dt in data:
                try:
                    if dt.isdigit():
                        commend.append(int(dt))
                    else:
                        commend.append(float(dt))
                except ValueError:
                    commend.append(None)
            
            if len(commend)>0:
                print("Received:%s"%commend)
                if commend[0]==0 and len(commend) == 10:
                    if commend[1] == 0:
                        mc.release_all_servos()
                        time.sleep(1)
                        try:
                            # self.wfile.write(str.encode("reless all servo"))
                            self.request.sendall(str.encode("reless all servo"))
                        except BrokenPipeError:
                            pass

                elif commend[0]==1 and len(commend) == 10:
                    # print(commend[0:2])
                    speed = commend[2]
                    if speed <= 0:
                        speed = 10
                    elif speed>=100:
                        speed = 100
                    if commend[1] == 3:                                         #偏移量控制
                        now_pos = mc.get_coords()
                        time.sleep(0.1)
                        if now_pos:
                            new_pos = commend[3: -1]
                            offset = [round(i + j, 3) for i,j in zip(now_pos, new_pos)]
                            print("target_coords: " + str(offset) + "speed: " + str(speed))
                            mc.send_coords(offset, speed, run_mode)
                            r_data = self.Get_Coords_parameters(offset, commend)
                            try:
                                # self.wfile.write(str.encode(str(r_data)))
                                self.request.sendall(str.encode(str(r_data)))
                            except BrokenPipeError:
                                pass          
                        else:
                            print("can not read now pos")
                            return 

                    elif commend[1] == 2:                                       #绝对坐标控制
                        now_pos = mc.get_coords()
                        time.sleep(0.1)
                        if now_pos:
                            offset = commend[3: -1]
                            print("target_coords: " + str(offset) + "speed: " + str(speed))
                            mc.send_coords(offset, speed, run_mode)
                            r_data = self.Get_Coords_parameters(offset, commend)
                            try:
                                # self.wfile.write(str.encode(str(r_data)))
                                self.request.sendall(str.encode(str(r_data)))
                            except BrokenPipeError:
                                pass
                        else:
                            print("can not read now pos")
                            return 

                    elif commend[1] == 1:                                       #读取当前坐标
                        now_pos = mc.get_coords()
                        time.sleep(0.1)
                        if now_pos:
                            offset = [round(i, 3) for i in now_pos]
                            print(offset)
                            mc.send_coords(offset, commend[2], run_mode)
                            r_data = ','.join([str(i) for i in commend[0:2]])
                            r_data += ',0,0,'
                            r_data += ','.join([str(j) for j in offset])
                            r_data += ',0'
                            try:
                                # self.wfile.write(str.encode(str(r_data)))
                                self.request.sendall(str.encode(str(r_data)))
                            except BrokenPipeError:
                                pass
                        else:
                            print("can not read pos")
                            return 
                
                elif commend[0]==2 and len(commend) == 10:
                    # print(commend[0:2])
                    speed = commend[2]
                    if speed <= 0:
                        speed = 10
                    elif speed>=100:
                        speed = 100
                    if commend[1] == 3:                                         #偏移角度控制
                        now_angles = mc.get_angles()
                        time.sleep(0.1)
                        if now_angles:
                            new_pos = commend[3: -1]
                            offset = [round(i + j, 3) for i,j in zip(now_angles, new_pos)]
                            print("target_angles: " + str(offset) + "speed: " + str(speed))
                            mc.send_angles(offset, speed)
                            r_data = self.Get_Angles_parameters(offset, commend)
                            try:
                                # self.wfile.write(str.encode(str(r_data)))
                                self.request.sendall(str.encode(str(r_data)))
                            except BrokenPipeError:
                                pass
                        else:
                            print("can not read now pos")
                            return 

                    elif commend[1] == 2:                                       #绝对角度控制
                        now_angles = mc.get_angles()
                        time.sleep(0.1)
                        if now_angles:
                            offset = commend[3: -1]
                            print("target_angles: " + str(offset) + "speed: " + str(speed))
                            mc.send_angles(offset, speed)
                            r_data = self.Get_Angles_parameters(offset, commend)
                            try:
                                # self.wfile.write(str.encode(str(r_data)))
                                self.request.sendall(str.encode(str(r_data)))
                            except BrokenPipeError:
                                pass
                        else:
                            print("can not read now pos")
                            return 

                    elif commend[1] == 1:                                       #读取当前角度
                        now_angles = mc.get_angles()
                        # print(now_angles)
                        time.sleep(0.1)
                        if now_angles:
                            offset = [round(i, 3) for i in now_angles]
                            print("Read_angles: " + str(offset))
                            r_data = ','.join([str(i) for i in commend[0:2]])
                            r_data += ',0,0,'
                            r_data += ','.join([str(j) for j in offset])
                            r_data += ',0'
                            try:
                                # self.wfile.write(str.encode(str(r_data)))
                                self.request.sendall(str.encode(str(r_data)))
                            except BrokenPipeError:
                                pass
                        else:
                            print("can not read now pos")
                            return 
                elif commend[0]==None or len(commend) != 10:                    #错误的数据error data
                    r_data = "error data: " + (','.join([str(i) for i in commend]))
                    print(r_data)
                    try:
                        # self.wfile.write(str.encode(r_data))
                        self.request.sendall(str.encode(str(r_data)))
                    except BrokenPipeError:
                                    pass
                time.sleep(0.01)

    def Get_Coords_parameters(self, offset, commend):                       #判断坐标误差范围是否符合
        sum = 0
        r_data = ''
        while True:
            now_pos_1 = mc.get_coords()
            time.sleep(1)
            if now_pos_1:
                offset_pos = [round(i - j, 3) for i,j in zip(offset, now_pos_1)]
                ar = np.array(offset_pos)
                if (abs(ar) < AMDR).all():
                    r_data = ','.join([str(i) for i in commend[0:2]])
                    r_data += ',0,0,'
                    r_data += ','.join([str(j) for j in now_pos_1])
                    r_data += ',0'
                    break
                else:
                    sum += 1
            else:
                sum += 1
            if sum > 5:
                r_data = ','.join([str(i) for i in commend[0:2]])
                r_data += ',1,0,'
                r_data += ','.join([str(j) for j in now_pos_1])
                r_data += ',0'
                break
        return r_data
                        
    def Get_Angles_parameters(self, offset, commend):                       #判断角度误差范围是否符合
        sum = 0
        r_data = ''
        while True:
            now_pos_1 = mc.get_angles()
            time.sleep(1)
            if now_pos_1:
                offset_angles = [round(i - j, 3) for i,j in zip(offset, now_pos_1)]
                ar = np.array(offset_angles)
                if (abs(ar) < AMDR).all():
                    r_data = ','.join([str(i) for i in commend[0:2]])
                    r_data += ',0,0,'
                    r_data += ','.join([str(j) for j in now_pos_1])
                    r_data += ',0'
                    break
                else:
                    sum += 1
            else:
                sum += 1
            if sum > 5:
                r_data = ','.join([str(i) for i in commend[0:2]])
                r_data += ',1,0,'
                r_data += ','.join([str(j) for j in now_pos_1])
                r_data += ',0'
                break
        return r_data


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
            print("server open seccess")
            server.serve_forever()
                        
        finally:
            print("server close")
            server.server_close()


if __name__ == "__main__":
    host = '192.168.10.144'                             #输入本机IP地址
    port = 8888
    server = SocketServer(host, port)                   #声明服务器端口
    try:
        mc = MyCobot('/dev/ttyAMA0', 1000000)           #树莓派版本打开机械臂，串口固定，无需USB连接
        # mc = MyCobot('COM5', 115200)                  #windows版本打开机械臂，串口序号查看设备管理器，需要USB连接
        run_mode = 2                                    #设定坐标运动模式，推荐模式2
        AMDR = 10                                       #设定误差允许范围，可自行设定，默认10mm/°
        print(mc)                                       #打印机械臂端口信息
    except:
        print("can not find cobot")
        exit(0)
    server.start_server()                               #永久打开服务器
