import time
from pymycobot.mycobot import MyCobot

data_id = [13, 21, 22, 23, 24, 26, 27]      
mode = 1                                # key : 1 or 2 
# mc = MyCobot('/dev/ttyAMA0', 1000000)   # raspi
mc = MyCobot('COM23', 115200)         # M5

for i in range(1,7):
    if mode == 1:
        data    = [80, 10, 0, 1, 0, 3, 3]
    elif mode == 2:
        if i < 4 :
            data = [80, 5, 15, 0, 0, 3, 3]
        else:
            data = [80, 8, 24, 0, 0, 3, 3]
    else:
        print("Please set the parameter mode !!!")
    
    for j in range(len(data_id)):
        mc.set_servo_data(i, data_id[j], data[j])
        time.sleep(0.2)
        _data = mc.get_servo_data(i, data_id[j])
        time.sleep(0.2)
        if _data == data[j]:
            print("Servo motor :" + str(i) + "  data_id : " + str(data_id[j]) + "   data: " + str(_data) + "  modify successfully ")
        else:
            print("Servo motor :"  + str(i) + "  data_id : " + str(data_id[j]) + "   data: " + str(_data) + "  modify error ")
