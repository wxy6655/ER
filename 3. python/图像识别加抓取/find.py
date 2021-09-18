#ecoding = utf-8
# Untitled - By: wxy - 周三 3月 17 2021

import sensor, image, time, math, lcd

import ujson
from Maix import GPIO
from fpioa_manager import fm, board_info


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA) # 如果分辨率太高，内存可能溢出
sensor.skip_frames(time = 2000)
#sensor.set_auto_gain(False)  # 必须关闭此功能，以防止图像冲刷…
#sensor.set_auto_whitebal(False)  # 必须关闭此功能，以防止图像冲刷…
clock = time.clock()
lcd.init()
lcd.rotation(2)

fm.register(board_info.LED_W, fm.fpioa.GPIO3)
led_w = GPIO(GPIO.GPIO3, GPIO.OUT)
led_w.value(0)

fm.register(34, fm.fpioa.UART2_TX, force=True)
fm.register(35, fm.fpioa.UART2_RX, force=True)
mySerial = UART(UART.UART2, 115200,8,0,0, timeout=20, read_buf_len= 4096)

#while(1):
    #mySerial.write(b'1231231')
    #time.sleep(2)


#blue,red,green,yellow

colour = ['blue','red','green','yellow']

colour_threshold =([20, 57, 1, 59, -48, -9],
                    [28, 49, 37, 127, 10, 66],
                    [42, 75, -61, -12, -16, 26],
                    [40, 85, -48, 4, 33, 81])
blobs = [0,0,0,0]

f_x = (2.8 / 3.984) * 160 # find_apriltags 如果没有设置，则默认为这个
f_y = (2.8 / 2.952) * 120 # find_apriltags 如果没有设置，则默认为这个
c_x = 160 * 0.5 # find_apriltags 如果没有设置，则默认为这个 (the image.w * 0.5)
c_y = 120 * 0.5 # find_apriltags 如果没有设置，则默认为这个 (the image.h * 0.5)

dict = {}
apr_date   = ['','','','']
blob_date  = ['','']
color_date = ''

def show_fps():
    fps =clock.fps()
    img.draw_string(200, 1, ("%2.2ffps" %(fps)),color=(255,0,0), scale=2)

def degrees(radians):
    return (180 * radians) / math.pi

def vision_init():
    global apr_data
    id_number = 0
    id_num    = 0
    apr_data = [0,0,0,0]
    apr = img.find_apriltags(fx=f_x, fy=f_y, cx=c_x, cy=c_y) # 默认为 TAG36H11
    if apr:
        for tag in apr:
            tmp = img.draw_rectangle(tag.rect(), color = (255, 0, 0))
            tmp = img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
            print_args = (tag.id(),tag.x_translation(), tag.y_translation(), tag.z_translation(), \
                degrees(tag.x_rotation()), degrees(tag.y_rotation()), degrees(tag.z_rotation()))
            id_num = tag.id()
            if id_num == 0:
                apr_data[0] = tag.cx()
                apr_data[1] = tag.cy()
            if id_num == 1:
                apr_data[2] = tag.cx()
                apr_data[3] = tag.cy()


def blobs_output():
    global blob_data, color_data
    for i in range(4):
        blobs = img.find_blobs([colour_threshold[i]],area_threshold=100,pixels_threshold=500)
        if blobs:
            color_data = str(colour[i])
            for b in blobs:
                tmp=img.draw_rectangle(b[0:4])
                tmp=img.draw_cross(b[5], b[6])
                img.draw_string(b[5], b[6], colour[i],color=(255,0,0), scale=2)
                c=img.get_pixel(b[5], b[6])
                blob_date[0] = b[5]
                blob_date[1] = b[6]

def Json_write():
    global apr_date, blob_date, color_date
    if all(apr_data) and all(blob_date) and color_date != '':
        dict["class"]       = color_date
        dict["base_date"]   = apr_date
        dict["target_date"] = blob_date
        encoded = ujson.dumps(dict)
        mySerial.write(str(encoded))
    else:
        encoded = {"class": " ", "base_date": [0, 0, 0, 0], "target_date": [0, 0]}
        mySerial.write(str(encoded))


while True:
    clock.tick()
    img = sensor.snapshot()
    vision_init()
    blobs_output()
    Json_write()
    show_fps()
    lcd.display(img)
