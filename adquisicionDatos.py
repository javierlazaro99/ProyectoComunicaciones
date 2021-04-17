import time
import struct
import board
import busio
import adafruit_ccs811
import serial

#Declaracion de variables
tiempo = 0.0;
TIEMPO_ENSAYO = 60
TIEMPO_LECTURA = 2.0

#Creamos las variables para controlar el bus i2c
i2c = busio.I2C(board.SCL, board.SDA)
ccs811 = adafruit_ccs811.CCS811(i2c)

#Creamos variables para controlar el puerto serie
ser = serial.Serial('/dev/rfcomm0')
ser.baudrate = 9600
print('Utilizando el puerto: ' + ser.name)

#Esperamos a que el sensor este ready
while not ccs811.data_ready:
    pass

#Capturamos el dato y lo escribimos por el puerto serie
#Envio de datos --> dato, dato, dato, .... ; El ; indica que se ha acabado un ciclo de lectura --> Indicador para el programa C de que se ha acabado la línea
print("Comunicación serie establecida, comenzando el ensayo...")
while tiempo <= TIEMPO_ENSAYO :
    try:
        print("CO2: %1.0f PPM" % ccs811.eco2)
        ser.write(struct.pack('I',ccs811.eco2))     #Enviamos el valor de C02 como unsigned integer
        ser.write(b',')                             #Separador
        ser.write(struct.pack('I',1))               #Enviamos el numero de personas
        ser.write(b',')                             #Separador
        ser.write(struct.pack('I',100))             #Enviamos el caudal de aire
        ser.write(b',')                             #Separador
        ser.write(struct.pack('f',tiempo))          #Enviamos el tiempo de la lectura como float
        ser.write(b';')                             #Final de mensaje
    except Exception as ex:
        print(ex.args[0])

    tiempo += TIEMPO_LECTURA
    time.sleep(TIEMPO_LECTURA)

print("Ensayo terminado, cerrando la comunicación serie...")
ser.close()
