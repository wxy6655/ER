'''
https://www.pythonf.cn/read/152939 Python机械手的坐标变换,python,臂,实现,末端,相对,移动

描述
假设机械臂末端执行器在pose1位置，x方向正好是它的左侧，想要让它向左侧移动10cm。显然我们直接下发命令，令它的 pose1.x = pose1.x + 0.1 就好了。

但是，如果我们的需求是机械臂在任何位姿下，都需要向它的左侧移动10cm呢（注意，任意位姿时它的正左侧不可能还是x轴正方向）

原理及解决方案
原理
机械臂有pose1和pose2两个位姿，两个6维位姿 {x, y, z, rx, ry, rz }都能得到各自的变换矩阵T1和T2
pose2位姿在pose1位姿坐标系下的表示，也就是变换矩阵T，可以使用公式 T2 = T1 * T 来表示
机械臂在pose3位姿时的变换矩阵是T3，则新的位姿pose4时的变换矩阵T4为 T4 = T3 * T
解决方案
记录机械臂在pose1的位姿，再记录一下正左侧pose2的位姿。
将pose1和pose2之间的变换矩阵T求出。
机械臂在任意pose3位姿时，只要根据已知的变换矩阵T，就能得到在pose3下正左侧的位姿pose4了

执行代码后，可以看出pose2比pose1z方向高了5cm，pose4比pose3同样高了5cm。注意，这里只是举例子，为了让大家看到代码是没问题的，
才使用了z方向。x、y、z本来都应该有变化的，但可以注意到rx、ry、rz在pose1、2、3、4都设置成了同样的数值，
因此pose4才只有z方向有变化
'''

import os,sys
sys.path.append(os.path.join(os.path.dirname(__file__),'../'))

import math
import numpy as np
import scipy.linalg as la
import time
from pymycobot.mycobot import MyCobot

# 矩阵里面的位置 一个4x4的旋转矩阵
# https://blog.csdn.net/u012424737/article/details/106269229
# 需要注意的也有：旋转矩阵的每一列表示的是对应的刚体坐标系的基在原参考坐标系下的表示，所以每一列之间是相互正交的，且模长为1，所以旋转矩阵是一个正交矩阵
	

def getPose_fromT(T):
	x = T[0, 3]
	y = T[1, 3]
	z = T[2, 3]
	rx = math.atan2(T[2, 1], T[2, 2])
	ry = math.asin(-T[2, 0]) 
	rz = math.atan2(T[1, 0], T[0, 0])

	return x, y, z, rx, ry, rz


def getT_fromPose1(pos):
	x = pos[0]
	y = pos[1]
	z = pos[2]
	rx = pos[3]
	ry = pos[4]
	rz = pos[5]
	return getT_fromPose(x, y, z, rx, ry, rz)

def getT_fromPose(x, y, z, rx, ry, rz):
	Rx = np.mat([[1, 0, 0], [0, math.cos(rx), -math.sin(rx)], [0, math.sin(rx), math.cos(rx)]])
	Ry = np.mat([[math.cos(ry), 0, math.sin(ry)], [0, 1, 0], [-math.sin(ry), 0, math.cos(ry)]])
	Rz = np.mat([[math.cos(rz), -math.sin(rz), 0], [math.sin(rz), math.cos(rz), 0], [0, 0, 1]])
	t = np.mat([[x], [y], [z]])

	R = Rz * Ry * Rx
	R_ = np.array(R)
	t_ = np.array(t)
	T_1 = np.append(R_, t_, axis = 1)
	
	zero = np.mat([0,0,0,1])
	T_2 = np.array(zero) 
	
	T = np.append(T_1, T_2, axis = 0)
	T = np.mat(T)

	return T


# 等待到达对应的位置
def waitPostion(bot,degrees,timeout=7):
    t = time.time()
    while time.time() - t < timeout:
        f = bot.is_in_position(degrees,1) # 0检查angles1检查coords
        if f:
            break
        time.sleep(0.1)

# 序列执行
def runOperation(bot,seqT):
#       循环变换矩阵序列
#           取得当前位置
#           根据计算当前位置矩阵，*seqT[i](第i个变换矩阵），计算取得新坐标，发送给机械臂（是否到位？等待，是否到位？等待，到位，退出等待）
#           i+=1;
    for seq in seqT:
        pos = bot.get_coords()
        T3 = getT_fromPose1(pos)
        print(T3)
        # 矩阵T3使用变换矩阵计算得到目标矩阵T4
        T4 = T3*seq
        # 根据目标矩阵T4取得坐标位置
        x4, y4, z4, rx4, ry4, rz4 = getPose_fromT(T4)
        print(x4,y4,z4,rx4,ry4,rz4)
        targetPos = [x4, y4, z4, rx4, ry4, rz4]
        # 机械臂移动到目标位置
        bot.send_coords(targetPos, 100,1)
        # 等待到达位置
        waitPostion(bot,targetPos)


# T2 = T1 * T
def getTransT_Pose2inPose1(T1, T2):
	return T1.I * T2

# 根据位置取得矩阵T1
T1 = getT_fromPose(-0.78399, -0.24807, 0.48833, 2.80385, -1.333807, -2.64379)
print(T1)
# 根据T1取得位置，应该等于上面的位置信息
x1, y1, z1, rx1, ry1, rz1 = getPose_fromT(T1)
print(x1,y1,z1,rx1,ry1,rz1)

# 根据位置取得矩阵T2
T2 = getT_fromPose(-0.78399, -0.24807, 0.53833, 2.80385, -1.333807, -2.64379)
print(T2)

# 根据T1和T2矩阵，计算两个位置的变换矩阵T，需要存下此变换矩阵，多个变换矩阵顺序执行，即为操作序列
T = getTransT_Pose2inPose1(T1, T2)
# 根据变换矩阵T取得变换矩阵T的位置 ？？还是转换矩阵T的三维坐标变换数？
x_T, y_T, z_T, rx_T, ry_T, rz_T = getPose_fromT(T)
print(T)
print(x_T,y_T,z_T,rx_T,ry_T,rz_T)


# 根据新的位置信息取得矩阵T3
T3 = getT_fromPose(-0.80693, 0.158207, 0.488374, 2.8036, -1.333807, 3.1395)
print(T3)
# 矩阵T3使用变换矩阵计算得到目标矩阵T4
T4 = T3*T
# 根据目标矩阵T4取得坐标位置
x4, y4, z4, rx4, ry4, rz4 = getPose_fromT(T4)
print(x4,y4,z4,rx4,ry4,rz4)

# 具体流程：
# 1，定位到某一个初始点的位姿，包括三维坐标及头部位姿
# 2，根据初始点位姿取得当前点的矩阵
# 3，当前点的矩阵*变换矩阵（）=目标点的矩阵
# 4，根据目标点的矩阵取得坐标位置
# 5，移动到目标点

# 序列流程：
# 1，深度摄像头出来的数据，服务器计算一个初始坐标点，例如胸部心脏位置；
# 2，服务器发送数据到机械臂，机械臂移动到位置，到位置后回服务器一个信号；
#         发送变换矩阵的序列（当前哪个标准/哪个医生的操作手法）
# 3，定位移动到服务端发来的位置姿态（是否到位？等待，是否到位？等待，到位，退出等待）
# 4，按序列执行（客户端上位机）
#       循环变换矩阵序列
#           取得当前位置
#           根据计算当前位置矩阵，*seqT[i](第i个变换矩阵），计算取得新坐标，发送给机械臂（是否到位？等待，是否到位？等待，到位，退出等待）
#           i+=1;
# 5，执行完成，回到默认位置

# 正上方拍照位置 [-146.0, -82.2, -26.2, 32.11, 1.44, -100.89] 



mycobot = MyCobot('com24')
time.sleep(5)
mycobot.set_free_mode()

mycobot.send_angles([0,0,0,0,0,0], 20)
time.sleep(5)

seqT = []
targetPos = [-145.3, -120.2, 5.3, 32.29, 6.65, -90.03]

t1 = [86.0, 39.9, -100.2, 68.73, 5.2, 177.13]
t2 = [-64.6, -22.7, -109.6, 78.97, 25.08, 28.9]
mycobot.send_coords(t1, 50,1)
time.sleep(5)
mycobot.send_coords(t2, 50,1)
T1 = getT_fromPose1(t1)
T2 = getT_fromPose1(t2)
T = getTransT_Pose2inPose1(T1, T2)
seqT.append(T)

# 串接上一个位置
t1 = [-116.0, -93.8, -40.4, 27.59, 19.24, -128.91]
mycobot.send_coords(t2, 100,1)
time.sleep(5)
mycobot.send_coords(t1, 100,1)
T1 = getT_fromPose1(t2)
T2 = getT_fromPose1(t1)
T = getTransT_Pose2inPose1(T1, T2)
seqT.append(T)

t2 = [-135.1, -102.1, 6.9, 62.63, 38.08, -103.88]
mycobot.send_coords(t1, 100,1)
time.sleep(5)
mycobot.send_coords(t2, 100,1)
T1 = getT_fromPose1(t1)
T2 = getT_fromPose1(t2)
T = getTransT_Pose2inPose1(T1, T2)
seqT.append(T)

t1 = [-88.5, -63.9, -76.6, 51.31, 17.49, -136.24]
mycobot.send_coords(t2, 100,1)
mycobot.send_coords(t1, 100,1)
T1 = getT_fromPose1(t2)
T2 = getT_fromPose1(t1)
T = getTransT_Pose2inPose1(T1, T2)
seqT.append(T)

t2 = [-160.3, -63.3, 6.6, 56.37, 20.52, -135.92]
mycobot.send_coords(t1, 100,1)
mycobot.send_coords(t2, 100,1)
T1 = getT_fromPose1(t1)
T2 = getT_fromPose1(t2)
T = getTransT_Pose2inPose1(T1, T2)
seqT.append(T)

t1 = [-95.2, -25.8, -94.3, 74.05, 11.04, -169.39]
mycobot.send_coords(t2, 100,1)
mycobot.send_coords(t1, 100,1)
T1 = getT_fromPose1(t2)
T2 = getT_fromPose1(t1)
T = getTransT_Pose2inPose1(T1, T2)
seqT.append(T)

t2 = [-150.9, -35.9, -27.9, 69.93, 23.92, -150.07]
mycobot.send_coords(t1, 100,1)
mycobot.send_coords(t2, 100,1)
T1 = getT_fromPose1(t1)
T2 = getT_fromPose1(t2)
T = getTransT_Pose2inPose1(T1, T2)
seqT.append(T)



# 机械臂移动到目标位置
mycobot.send_coords(targetPos, 100,1)
# 等待到达位置
waitPostion(mycobot,targetPos)

# 按序列执行操作
runOperation(mycobot,seqT)

# 执行完回归直立状态
mycobot.send_coords([45.9, -88.1, 462.2, -93.03, -10.15, -92.97], 100,1)