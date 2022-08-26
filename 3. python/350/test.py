from pymycobot.mycobot import MyCobot
import time


mc = MyCobot('COM5',115200)
mc.power_on()
time.sleep(3)
mc.send_angles([0,0,0,0,0,0],50)
time.sleep(3)
mc.send_angles([(-15.11),2.1,(-88.5),(-0.79),92.28,(-57.04)],50)
time.sleep(3)
mc.send_coords([183.3,(-137.7),255.6,(-170),(-1.11),(-48.05)],50,0)
time.sleep(3)
mc.send_coords([233.3,(-137.7),255.6,(-170),(-1.11),(-48.05)],50,1)