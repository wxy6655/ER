from pymycobot.mycobot import MyCobot
import time


mc = MyCobot('COM6',115200)
mc.set_color(255,0,0)
time.sleep(3)
mc.set_color(255,255,0)
time.sleep(3)
mc.set_color(255,255,255)
time.sleep(3)