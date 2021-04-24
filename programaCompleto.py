import RPi.GPIO as GPIO
import time
import struct
import board
import busio
import adafruit_ccs811
import serial
import os

#Declaracion de variables
GPIO_PIN_PERSONAS = 23
GPIO_PIN_AIRE = 24
GPIO_PIN_RELE = 21
tiempo = 0.0;
puerto_abierto = False
rele_activo = False
TIEMPO_LECTURA = 2.0
num_personas = 1
caudal = 0

start_time = time.time()

#Creamos las variables para controlar el bus i2c
i2c = busio.I2C(board.SCL, board.SDA)
ccs811 = adafruit_ccs811.CCS811(i2c)

#Esperamos a que el sensor esté listo
while not ccs811.data_ready:
    pass

#Creamos el control para los GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(GPIO_PIN_PERSONAS, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(GPIO_PIN_AIRE, GPIO.IN, pull_up_down = GPIO.PUD_UP)
GPIO.setup(GPIO_PIN_RELE, GPIO.OUT)
GPIO.output(GPIO_PIN_RELE, False)

#Funciones para los eventos de botones
def sumarPersonas(null):
    global num_personas
    if num_personas < 4:
        num_personas+=1
    else:
        num_personas = 1

def activarVentilador(null):
    global rele_activo
    global caudal
    if not rele_activo: #Rele no activo --> Se activa el rele
        GPIO.output(GPIO_PIN_RELE, True)
        caudal = 100
        rele_activo = True
    else: #Rele activo --> Se desactiva el rele
        GPIO.output(GPIO_PIN_RELE, False)
        caudal = 0
        rele_activo = False

GPIO.add_event_detect(GPIO_PIN_PERSONAS, GPIO.FALLING, callback=sumarPersonas, bouncetime=300)
GPIO.add_event_detect(GPIO_PIN_AIRE, GPIO.FALLING, callback=activarVentilador, bouncetime=300)

#Bucle infinito de monitorización de datos: CO2, personas y caudal
try:
    while 1:
        try:
            co2 = ccs811.eco2
        except Exception as ex:
            pass

        #Leemos constantemente, pero solo mostramos por pantalla cada 2s más o menos
        if (time.time() - start_time) >= 2.0:
            print("CO2: %1.0f PPM, Nº Personas: %d, Caudal: %3d m3" % (co2, num_personas, caudal))
            start_time = time.time()

        #Comprobamos si se puede establecer conexión
        if puerto_abierto == False:
            try:
                #Creamos variables para controlar el puerto serie
                ser = serial.Serial('/dev/rfcomm0')
                ser.baudrate = 9600
                ser.timeout = 20
                print('Utilizando el puerto: ' + ser.name)
                puerto_abierto = True
            except Exception as ex:
                pass
        else:
            #Esperamos a recibir el mensaje de inicio de experimento
            try:
                line = ser.readline().decode("ascii")
                if line.startswith("START"):
                    #Dividimos el string por la coma, para obtener el valor de tiempo indicado
                    split = line.strip().split(',')
                    tiempo_experimento = int(split[1]) * 60.0

                    print("Comunicación serie establecida, comenzando el ensayo durante %d segundos..." % tiempo_experimento)

                    while tiempo <= tiempo_experimento :
                        try:
                            print("Tiempo: %.1f s, CO2: %1.0f PPM, Nº Personas: %d, Caudal: %3d m3" % (tiempo, ccs811.eco2, num_personas, caudal))
                            ser.write(struct.pack('I',ccs811.eco2))     #Enviamos el valor de C02 como unsigned integer
                            ser.write(b',')                             #Separador
                            ser.write(struct.pack('I',num_personas))               #Enviamos el numero de personas
                            ser.write(b',')                             #Separador
                            ser.write(struct.pack('I',caudal))             #Enviamos el caudal de aire
                            ser.write(b',')                             #Separador
                            ser.write(struct.pack('f',tiempo))          #Enviamos el tiempo de la lectura como float
                            ser.write(b';')                             #Final de mensaje
                        except Exception as ex:
                            print(ex.args[0])

                        tiempo += TIEMPO_LECTURA
                        time.sleep(TIEMPO_LECTURA)

                    print("Ensayo terminado, cerrando la comunicación serie...")
                    tiempo = 0.0
                    puerto_abierto = False
                    ser.close()
                    os.system("sudo rfcomm release 0")
                    time.sleep(1)
            except Exception as ex:
                print(ex)
except KeyboardInterrupt:
        GPIO.cleanup()
