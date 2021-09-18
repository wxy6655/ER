#ecoding = utf-8
from m5stack import *
from m5ui import *
from uiflow import *
from ThirdParty.myCobot import myCobot
from easyIO import *
import time

setScreenColor(0x222222)

serial_data = None

label0_0 = M5TextBox(0, 0, " ", lcd.FONT_Default, 0xFFFFFF, rotate=0)
label0_1 = M5TextBox(70, 0, " ", lcd.FONT_Default, 0xFFFFFF, rotate=0)
label0_2 = M5TextBox(150, 0, " ", lcd.FONT_Default, 0xFFFFFF, rotate=0)
label0_3 = M5TextBox(220, 0, " ", lcd.FONT_Default, 0xFFFFFF, rotate=0)

label1 = M5TextBox(0, 40, " ", lcd.FONT_Default, 0xFFFFFF, rotate=0)
label2 = M5TextBox(0, 80, " ", lcd.FONT_Default, 0xFFFFFF, rotate=0)
label3 = M5TextBox(0, 120, " ", lcd.FONT_Default, 0xFFFFFF, rotate=0)
label4 = M5TextBox(0, 160, " ", lcd.FONT_Default, 0xFFFFFF, rotate=0)
label5 = M5TextBox(100, 160, " ", lcd.FONT_Default, 0xFFFFFF, rotate=0)

pin0 = machine.Pin(2, mode=machine.Pin.OUT, pull=machine.Pin.PULL_UP)
pin1 = machine.Pin(5, mode=machine.Pin.OUT, pull=machine.Pin.PULL_UP)

pos_init_01 = [ -8,170, 50, 90, 13, 95]
pos_init_02 = [ -8,-170, 50, 90, 13, 95]
pos_init_1 = [  5, 146, 49, 92, 16, -67]
pos_init_2 = [132, 217, 46, 92, 16, -67]
pos_init_3 = [ 50,  50, 46, 92, 16, -67]

time_out = 50
path = []

path_1 = [[95, -51, -55, -59, 95, -8],
          [75, -51, -55, -59, 95, -8],
          [50, -51, -55, -59, 95, -8],
          [25, -51, -55, -59, 95, -8],
          [ 0, -51, -55, -59, 95, -8]]
          
path_2 = [[-95, -51, -55, -59, 95, -8],
          [-75, -51, -55, -59, 95, -8],
          [-50, -51, -55, -59, 95, -8],
          [-25, -51, -55, -59, 95, -8],
          [ 0, -51, -55, -59, 95, -8]]


pos_1_x = 0
pos_1_y = 0
pos_2_x = 0
pos_2_y = 0
pos_3_x = 0
pos_3_y = 0

color = 0

direction = 0

runing_state = False

kx = 0
ky = 0

cobot = myCobot.myCobot()
uart1 = machine.UART(2, tx=21, rx=22)
uart1.init(115200, bits=8, parity=None, stop=1)
digitalWrite(2, 1)
digitalWrite(2, 1)
pos1 = [[],[]]


def sp_closs():
  pin0.on()
  wait_ms(200)
  pin1.on()
  wait_ms(800)        #

def sp_open():
  pin0.off()
  wait_ms(800)
  pin1.off()
  wait_ms(200)

def run(pos1, state):
  pos2 = pos1.copy()
  pos2[2] += 70
  cobot.send_coords(pos2, 70, 0)
  wait(5)
  for _ in range(15):
    pos2[2] -= 5
    cobot.send_coords(pos2, 70, 0)
    wait_ms(50)
  if state:
    sp_open()
  else:
    sp_closs()
  wait(2)
  for _ in range(50):
    pos2[2] += 2
    cobot.send_coords(pos2, 70, 0)
    wait_ms(50)
  wait(5)

def Serial_read():
  color_old = 0
  color_new = 0
  x_old = 0
  x_new = 0
  y_old = 0
  y_new = 0
  read_sum = 0
  uart1.read()
  time_old = time.time()
  while(1):
    if (uart1.any()) > 0:
      serial_data = uart1.read()
      count = 0
      index = 0
      if '][' in serial_data:
        for i in range(len(serial_data)):
          if serial_data[i] == '[':
            count += 1
            label0_3.setText('5'+str(count))
          if count == 2:
            index = i
        serial_data = serial_data[:index]
        serial_data = list(str(str(serial_data)[3:-2]).split(', '))
      else:
        serial_data = list(str(str(serial_data)[3:-2]).split(', '))
      if len(serial_data) > 8:
        data_1 = [int(serial_data[0]), int(serial_data[1]), int(serial_data[2]),
                  int(serial_data[3]), int(serial_data[4]), int(serial_data[5]),
                  int(serial_data[6]), int(serial_data[7]), int(serial_data[8])]
        color_old = color_new 
        color_new = data_1[6]
        x_old = x_new
        x_new = data_1[7]
        y_old = y_new
        y_new = data_1[8]
        if (color_new - color_old) == 0:
          dx = x_new - x_old
          dy = y_new - y_old
          d_sum = (dx+dy)/2
          if d_sum < 10:
            read_sum += 1 
            if read_sum > 10:
              return data_1
          else:
            continue
      else:
        continue
    continue
  
def get_kx_ky():
  global kx,ky,color
  data = Serial_read()
  if data:
    if data[0]:
      pos_1_x = round(data[1], 2)
      pos_1_y = round(data[2], 2)
    if data[3]:
      pos_2_x = round(data[4], 2)
      pos_2_y = round(data[5], 2)
    if data[6]:
      color = data[6]
      pos_3_x = round(data[7], 2)
      pos_3_y = round(data[8], 2)
    try:
      kx = round(((pos_3_x-pos_1_x)/(pos_2_x-pos_1_x)),2)
      ky = round(((pos_3_y-pos_1_y)/(pos_2_y-pos_1_y)),2)
    except Exception as e:
      wait_ms(50)
      get_kx_ky()
      return
    label0_0.setText('kx:' + str(kx))
    label0_1.setText('ky:' + str(ky))
    label0_2.setText('x3:' + str(pos_3_x))
    label0_3.setText('y3:' + str(pos_3_y))
    return True
  else:
    return False  
    
def runing():
  global pos_init_3,pos_init_1,pos_init_2,direction,path_1,path_2,path
  if runing_state:
    pos_init_3 = pos_init_1.copy()
    pos_init_3[0] = kx*(pos_init_2[0] - pos_init_1[0]) + pos_init_1[0]
    pos_init_3[1] = ky*(pos_init_2[1] - pos_init_1[1]) + pos_init_1[1]
    label2.setText(str(pos_init_1))
    if pos_init_3[0] > 400 or pos_init_3[0] < -400:
      label3.setText('plase move color to right pos')
      # return False
    else:
      if direction == 0:
        for k in range(5):
          cobot.send_angles(path_1[k], 20)
          wait(1)
      elif direction == 1:
        for k in range(5):
          cobot.send_angles(path_1[k], 20)
          wait(1)
      elif direction == 2:
        for k in range(5):
          cobot.send_angles(path_2[k], 20)
          wait(1)
      wait(2)
      run(pos_init_3, 1)
      if 0 < color < 3:
        path = path_1.copy()
        pos_init = pos_init_01.copy()
        direction == 1
      else:
        path = path_2.copy()
        pos_init = pos_init_02.copy()
        direction == 2
      for k in range(5):
        cobot.send_angles(path[4-k], 20)
        wait_ms(1000)
      run(pos_init, 0)
  else:
    label1.setText('please btnA write pos init')
  
def Cralbration_Pos():
  cp = 0
  pos = [[],[]]
  label1.setText('Please btnA write pos')
  while (1):
    cs = cobot.get_coords()
    wait_ms(100)
    # cs1 = cobot.get_angles()
    # wait_ms(100)
    if btnA.wasReleased():
      wait_ms(50)
      if cs:
        if cp > 1:
          global runing_state, pos_init_1, pos_init_2
          runing_state = True
          pos_init_1 = pos[0]
          pos_init_2 = pos[1]
          label1.setText('Cralbration_Pos OK')
          break
        else:
          pos[cp] = cs
          label1.setText('Cralbration_Pos ' + str(cp) + ' OK') 
          label2.setText(str(pos[cp]))
          # label3.setText(str(cs1))
          cp += 1
      else:
        label1.setText('Please RebtnA write pos')

def init():
  info()
  sp_closs()
  wait_ms(50)
  cobot.send_angles(path_1[0],50)
  wait(3)
  
def info():
  label1.setText('Please btnA INIT ')
  label2.setText('Please btnB START')
  label3.setText('Please btnC RELESS')

init()
info()
while True:
  if btnA.wasReleased():
    Cralbration_Pos()
  if btnB.wasReleased():
    if runing_state:
      if get_kx_ky():
        runing()
    else:
      label1.setText('Please FARST btnA init pos')
      wait(2)
      info()
  if btnC.wasReleased():
    cobot.set_free_mode()
    wait_ms(100)
    label1.setText('reless secces')
  wait_ms(2)
