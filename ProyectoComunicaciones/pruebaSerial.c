#include <stdio.h>
#include <Windows.h>

HANDLE abrirPuertoSerie() {

}

//Combina 4 bytes en un unsigned int
unsigned int combinarAInt(unsigned char* bytes, int inicio) {
    unsigned int ret = bytes[inicio] | (bytes[inicio + 1] << 8) | (bytes[inicio + 2] << 16) | (bytes[inicio + 3] << 24);
    return ret;
}

//Combina 4 bytes a un float
void conbinarAFloat(unsigned char* bytes, int inicio, float *f) {

    unsigned char bytes_aux[4] = { bytes[inicio],bytes[inicio + 1], bytes[inicio + 2],bytes[inicio + 3] };
    memcpy(f, bytes_aux, sizeof(float));
}

int main() {

    HANDLE port;
    char* device = "\\\\.\\COM6";
    char buffer[5];
    DWORD bytesLeidos;

    //Con esto tratamos de abrir el puerto COM6
    port = CreateFileA(device, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (port == INVALID_HANDLE_VALUE) {
        printf("Error al abrir el puerto: %s", device);
        CloseHandle(port);
        return(0);
    }
    
    //Con esto nos ponemos a leer lo que nos llega por el COM6
    //Nos vamos a leer 5 bytes 5*sizeof(char) que es 1 byte
    
   // ReadFile(port, buffer, sizeof(char)*5, &bytesLeidos, NULL);

     
   // printf("%s", buffer);

    //Configuramos los parametros del puerto
    DCB dcbConf;
    if (GetCommState(port, &dcbConf)) {
        dcbConf.BaudRate = 9600;
        dcbConf.ByteSize = 8;
        dcbConf.Parity = NOPARITY;
        dcbConf.StopBits = ONESTOPBIT;
        dcbConf.fBinary = TRUE;
        dcbConf.fParity = TRUE;
    }
    else {
        printf("Error en la configuración del puerto: %s", device);
        CloseHandle(port);
        return(0);
    }

    SetCommState(port, &dcbConf);

    //Configuramos la estructura de timeouts
    COMMTIMEOUTS cTimeout;

    if (GetCommTimeouts(port, &cTimeout)) {
        cTimeout.ReadIntervalTimeout = 0;
        cTimeout.ReadTotalTimeoutConstant = 1000;
        cTimeout.ReadTotalTimeoutMultiplier = 0;
        cTimeout.WriteTotalTimeoutConstant = 1000;
        cTimeout.WriteTotalTimeoutMultiplier = 0;
    }
    else {
        printf("Error en la configuración de los timeouts: %s", device);
        CloseHandle(port);
        return(0);
    }

    SetCommTimeouts(port, &cTimeout);

    //Configurar la lectura
    DWORD eventMask;

    SetCommMask(port, EV_RXCHAR); //Establecemos el evento a cualquier caracter recibido

    unsigned char c;
    unsigned char cBuf[10];
    DWORD readSize;
    int size = 0;
    int msg = 1;

    do {
        //Esperamos al evento de lectura por el SO
        if (WaitCommEvent(port, &eventMask, NULL)) {
            //Intentamos leer 1 byte y nos lo guardamos en nuestro array de bytes final, 
            //así hasta que no quede nada por leer
            do {
                if (ReadFile(port, &c, 1, &readSize, NULL) != 0) {
                    if (readSize > 0) {
                        if (c == '/') {
                            msg = 0; //Flag que indica que se acabó el mensaje
                            printf("Se acabo\n");
                            break;
                        }
                        size += readSize;
                        cBuf[size - 1] = c;
                    }
                }
                else {
                    printf("Error en la lectura del puerto");
                    CloseHandle(port);
                    return(0);
                }
                
            } while (c != ';');

            unsigned int a = combinarAInt(cBuf, 0);
            unsigned char b = cBuf[4];
            float d;
            conbinarAFloat(cBuf, 5, &d);
            unsigned char e = cBuf[9];

            printf("Valores leidos: %d%c%.1f%c\n", a, b, d, e);
            size = 0;
        } 

        else {
            printf("Error al detectar el evento de comunicaciones");
            CloseHandle(port);
            return(0);
        }
        
    } while (msg);

    CloseHandle(port);
    port = INVALID_HANDLE_VALUE;
    return(0);
}
