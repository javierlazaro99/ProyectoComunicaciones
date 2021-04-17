import time
import struct
import board
import busio
import adafruit_ccs811
import serial

#Declaracion de variables
tiempo = 0.0;
puerto_abierto = False
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

#Bucle infinito de monitorización de datos: CO2, personas y caudal
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
            ser.timeout = 1
            print('Utilizando el puerto: ' + ser.name)
            puerto_abierto = True
        except Exception as ex:
            pass
    else:
        #Esperamos a recibir el mensaje de inicio de experimento
        line = ser.readline().decode("ascii")
        if line.startswith("START"):
            #Dividimos el string por la coma, para obtener el valor de tiempo indicado
            split = line.strip().split(',')
            tiempo_experimento = int(split[1]) * 60.0

            print("Comunicación serie establecida, comenzando el ensayo durante %d segundos..." % tiempo_experimento)

            while tiempo <= tiempo_experimento :
                try:
                    print("CO2: %1.0f PPM, Nº Personas: %d, Caudal: %3d m3" % (ccs811.eco2, num_personas, caudal))
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
            break
