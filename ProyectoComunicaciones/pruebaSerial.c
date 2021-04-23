#include <stdio.h>
#include <Windows.h>
#include "comunicaciones.h"

#pragma warning(disable:4996)


unsigned int combinarAInt(unsigned char* bytes, int inicio) {
    unsigned int ret = bytes[inicio] | (bytes[inicio + 1] << 8) | (bytes[inicio + 2] << 16) | (bytes[inicio + 3] << 24);
    return ret;
}

void combinarAFloat(unsigned char* bytes, int inicio, float *f) {

    unsigned char bytes_aux[4] = { bytes[inicio],bytes[inicio + 1], bytes[inicio + 2],bytes[inicio + 3] };
    memcpy(f, bytes_aux, sizeof(float));
}

int combinarBuffer(DATO *miDato,char* buffer, int tamBuffer, int numSep, char fin) {
    int sep = 0, error = 0;

    //Comprobación de que el buffer es válido 
    for (int i = 0; i < tamBuffer; i++)
    {
        if (buffer[i] == ',') {
            sep++;
        }
    }

    //Buffer con características válidas, hay que checkear validez de los datos
    if ((sep == numSep) ) { 
        miDato->co2 = combinarAInt(buffer, 0);
        miDato->numPersonas = combinarAInt(buffer, 5);
        miDato->caudal = combinarAInt(buffer, 10);
        combinarAFloat(buffer, 15, &miDato->tiempo);
    }
    else { //Buffer invalido
        error = 1;
    }

    //Checkear la validez de los datos leídos
    if (miDato->co2 < 400) {
        error = 1; //Lecturas por debajo de 400 son errores del sensor
    }

    return(error);
}


int abrirPuerto(HANDLE *port) {

    //HANDLE port;
    char* device = "\\\\.\\COM6";

    //Con esto tratamos de abrir el puerto COM6
    *port = CreateFileA(device, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (*port == INVALID_HANDLE_VALUE) {
        printf("Error al abrir el puerto: %s", device);
        CloseHandle(*port);
        return(0);
    }

    //Configuramos los parametros del puerto
    DCB dcbConf;
    if (GetCommState(*port, &dcbConf)) {
        dcbConf.BaudRate = 9600;
        dcbConf.ByteSize = 8;
        dcbConf.Parity = NOPARITY;
        dcbConf.StopBits = ONESTOPBIT;
        dcbConf.fBinary = TRUE;
        dcbConf.fParity = TRUE;
    }
    else {
        printf("Error en la configuración del puerto: %s", device);
        CloseHandle(*port);
        return(0);
    }

    SetCommState(*port, &dcbConf);

    //Configuramos la estructura de timeouts
    COMMTIMEOUTS cTimeout;

    if (GetCommTimeouts(*port, &cTimeout)) {
        cTimeout.ReadIntervalTimeout = 5; // Tiempo de espera entre recibir 2 bytes conecutivos,
        cTimeout.ReadTotalTimeoutConstant = 5000; // Cuanto tiempo se queda esperando el readfile
        cTimeout.ReadTotalTimeoutMultiplier = 0; // Esto da un poco igual --> No se usa mucho
        cTimeout.WriteTotalTimeoutConstant = 1000;
        cTimeout.WriteTotalTimeoutMultiplier = 0;
    }
    else {
        printf("Error en la configuración de los timeouts: %s", device);
        CloseHandle(*port);
        return(0);
    }

    SetCommTimeouts(*port, &cTimeout);

    return(1);
}

int comenzarExperimento(HANDLE port, int time) {

    char msg[10];
    sprintf(msg, "START,%d\n", time);
    int numBytes;
    if (WriteFile(port, msg, 10 , &numBytes, NULL) != 0) {
        printf("Comenzando experimento con t = %d minutos...\n", time);
    }
    else {
        printf("Error en la escritura del puerto");
        CloseHandle(port);
        return(0);
    }


    return(1);
}

int leerDatosExperimento(HANDLE port, DATO** listaDatos, int* numDatos) {   
    //Configurar la lectura
    DWORD eventMask;

    SetCommMask(port, EV_RXCHAR); //Establecemos el evento a cualquier caracter recibido

    //-------------------------- LECTURA PUERTO SERIE ---------------------------------------//

    unsigned char bufferLectura[100];
    int continuar = 1, contDatos = 0;
    DWORD readSize;
    DATO miDato;
    if (*listaDatos == NULL) {
        printf("Error en la creación de la lista de datos");
        CloseHandle(port);
        return(0);
    }

    do {
        //Lanzamos una lectura del puerto serie
        if (ReadFile(port, bufferLectura, 100, &readSize, NULL) != 0) {
            //Si en menos de 5s hemos leído algo (puede ser vacío) saltaremos aquí
            if (readSize == 0) {
                //Tras 5s no hemos leído nada --> Salir bucle
                printf("Ensayo finalidado, saliendo...\n");
                continuar = 0;
            }
            else {
                //Hemos leído algo y lo tenemos en el buffer --> Procesarlo y guardarlo
                if (combinarBuffer(&miDato, bufferLectura, readSize, 3, ';') == 0) {
                    printf("Valores leidos: CO2: %d  , Num Personas: %d  , Caudal aire: %d , Tiempo: %.1f ;\n", miDato.co2, miDato.numPersonas, miDato.caudal, miDato.tiempo);
                    (*listaDatos)[contDatos] = miDato;
                    contDatos++;
                }
                else {
                    printf("Error, buffer no valido\n");
                }
            }
        }
        else {
            printf("Error en la lectura del puerto");
            CloseHandle(port);
            return(0);
        }
    } while (continuar);

    *numDatos = contDatos;

    CloseHandle(port);
    port = INVALID_HANDLE_VALUE;
    return(1);
}
