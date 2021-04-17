import time
import struct
import board
import busio
import adafruit_ccs811
import serial

puerto_abierto = False

while 1:
    if puerto_abierto == False:
        try:
            ser = serial.Serial('/dev/rfcomm0')
            ser.baudrate = 9600
            ser.timeout = 1
            print('Utilizando el puerto: ' + ser.name)
            puerto_abierto = True
        except Exception as ex:
            pass
    else
        line = ser.readline().decode("ascii")
        if line.startswith("START"):
            split = line.strip().split(',')
            print(split[0])
            print(split[1])
            break
