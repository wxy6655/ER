#encoding: UTF-8
#!/usr/bin/env python2
# import cv2 as cv
import os
import time
from pymycobot.mycobot import MyCobot
import math


class Detect_marker(object):
    def __init__(self):
        super(Detect_marker, self).__init__()

        self.mc = MyCobot("COM42", 115200)
        self.mc.power_on()
       

    def grap(self, flag):
        if flag:
            # close
            # self.mc.set_gripper_state(1, 0)
            self.mc.set_gripper_value(40,0)
            time.sleep(2)
        else:
            # open
            self.mc.set_gripper_state(0, 0)
            time.sleep(2)

    def move_to_target_coords(self,coords,speed):
        print("move_to_target_coords")
        self.mc.send_coords(coords,speed,1)
        time.sleep(5)

    # Grasping motion
    def run(self):
        
        coords  = [210, -59.3, 245, -178.51, 0.28, -135]
        height_bias = 148
        self.move_to_target_coords([coords[0],coords[1],height_bias,coords[3],coords[4],coords[5]], 15)
        self.grap(True)   
        print("close")     

        self.move_to_target_coords(coords, 15)
        time.sleep(2)

        self.move_to_target_coords([coords[0],coords[1],height_bias+5,coords[3],coords[4],coords[5]], 15)
        self.grap(False)
        print("open")

        self.move_to_target_coords(coords, 15)
        time.sleep(2)




if __name__ == "__main__":
    detect = Detect_marker()
    while True:
        detect.mc.set_color(0,0,255)#blue, arm is busy
        detect.run()
        detect.mc.set_color(0,255,0)#green, arm is free
    
