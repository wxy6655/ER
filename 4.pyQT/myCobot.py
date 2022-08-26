from distutils.cmd import Command
from re import S
import sys, os
import pathlib
import serial
import subprocess
import copy
# import logo
import threading
import time
sys.path.append('.')

import serial.tools.list_ports
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from Ui_mainwindow import Ui_myCobot
from esptool import main as esptool

class ProgramWindow(QMainWindow, Ui_myCobot):
    command = ['--chip', 'esp32', '--port', '', '--baud', '',
            '--before', 'default_reset', '--after', 'hard_reset', 'write_flash', '-z', '--flash_mode', 'dio', '--flash_freq', '80m', '--flash_size', 'detect',
            '0xe000', '',
            '0x1000', '',
            '0x10000', '',
            '0x8000', '']

    def __init__(self):
        super(self.__class__, self).__init__()
        self.setupUi(self)
        self.setup_connection()
        self.threading_control()
        self.setWindowTitle("myCobot-程序下载器")
        self.setWindowIcon(QIcon(QIcon(':/name.ico')))

    def setup_connection(self):
        self.burn.clicked.connect(self.burn_bin)

    def port_show(self):
        self.port.clicked.connect(self.auto_serial)

    def _a(self):
        esptool()

    def burn_bin(self):
        port_number = self.port.currentText()
        plank_name = self.plank.currentText()
        file_name = self.file.currentText()
        if plank_name == 'Atom':
            baud = '1500000'
        else:
            baud = '921600'
        path = os.getcwd()
        _command = self.command.copy()
        _command[3]  = str(port_number)
        _command[5]  = baud
        _command[-7] = path + '/boot/boot_app0.bin'
        _command[-5] = path + '/boot/bootloader_qio_80m.bin'
        _command[-3] = path + '/ino/' + file_name +'.bin'
        _command[-1] = path + '/boot/default.bin'
        while True:
            num = 0
            try:
                esptool(_command)
                break
            except Exception as a:
                if num > 20:
                    print(a)
                    break
                else:
                    num+=1
                    time.sleep(0.02)
                    continue

    def auto_serial(self):
        self.port.clear()
        port_list = list(serial.tools.list_ports.comports())
        num_0 = 0
        num_1 = 0
        while True:
            if len(port_list) <= 0:
                self.port.clearEditText()
                self.port.clear()
                self.port.addItem('NO PORT')
                num_0 = 0
                num_1 = 0
                print ("The Serial port can't find!")
            else:
                num_0 = num_1
                num_1 = len(port_list)
                if num_0 != num_1:
                    self.port.clear()
                    for i in range(len(port_list)):
                        port_list_0 = port_list[i]
                        self.port.addItem(port_list_0[0])
            time.sleep(1)
            port_list = list(serial.tools.list_ports.comports())
            

    def auto_file(self):
        file_list = self.file_name()
        if len(file_list) <= 0:
            print ("The file can't find!")
        else:
            for i in range(len(file_list)):
                port_list_0 = file_list[i]
                self.file.addItem(port_list_0[:-4])

    def file_name(self):   
        path_ = str(os.getcwd()) + '/ino'
        for root, dirs, files in os.walk(path_, topdown=False): 
            _f = filter(lambda x: x.endswith('.bin'), files)
            return(list(_f))
        else:
            return []
    
    def threading_control(self):
        serial = threading.Thread(target=self.auto_serial)
        serial.setDaemon(True)
        serial.start()
        
        file = threading.Thread(target=self.auto_file)
        file.setDaemon(True)
        file.start()
    

if __name__ == "__main__":
    app = QApplication(sys.argv)
    pw = ProgramWindow()
    pw.show()
    sys.exit(app.exec())
    
    