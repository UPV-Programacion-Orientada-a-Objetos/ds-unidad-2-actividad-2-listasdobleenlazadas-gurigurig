/**
 * @file SerialPort.cpp
 * @brief Implementación de la clase SerialPort para comunicación serial REAL.
 */

#include "SerialPort.h"
#include <iostream>
#include <cstdlib>  // Para malloc, free
#include <cstring>  // Para memset

#ifndef _WIN32
    #include <fcntl.h>  // Para flags de apertura de archivo
#endif

/**
 * @brief Implementación manual de strlen.
 */
static size_t manual_strlen(const char* str) {
    size_t len = 0;
    while (str && str[len] != '\0') {
        len++;
    }
    return len;
}

/**
 * @brief Implementación manual de strcpy.
 */
static char* manual_strcpy(char* dest, const char* src) {
    if (dest == nullptr || src == nullptr) return nullptr;
    char* original_dest = dest;
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return original_dest;
}

SerialPort::SerialPort() : connected(false), bufferPos(0) {
#ifdef _WIN32
    hSerial = INVALID_HANDLE_VALUE;
#else
    fd = -1;
#endif
    memset(readBuffer, 0, sizeof(readBuffer));
}

SerialPort::~SerialPort() {
    close();
}

bool SerialPort::open(const char* portName, int baudRate) {
    if (connected) {
        return true; // Ya está conectado
    }

#ifdef _WIN32
    // ===== WINDOWS IMPLEMENTATION =====
    // Convertir "COM3" a "\\.\COM3" para puertos > COM9
    char fullPortName[20];
    if (portName[0] == 'C' && portName[1] == 'O' && portName[2] == 'M') {
        snprintf(fullPortName, sizeof(fullPortName), "\\\\.\\%s", portName);
    } else {
        manual_strcpy(fullPortName, portName);
    }

    // Abrir el puerto
    hSerial = CreateFileA(fullPortName,
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: No se pudo abrir el puerto " << portName << std::endl;
        DWORD error = GetLastError();
        std::cerr << "Código de error: " << error << std::endl;
        return false;
    }

    // Configurar parámetros del puerto
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error: No se pudo obtener el estado del puerto" << std::endl;
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        return false;
    }

    // Configurar: Baud rate, 8 bits, sin paridad, 1 bit de parada
    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error: No se pudo configurar el puerto" << std::endl;
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        return false;
    }

    // Configurar timeouts
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error: No se pudieron configurar los timeouts" << std::endl;
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        return false;
    }

    // Limpiar el buffer
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

    connected = true;
    std::cout << "Conexión establecida con " << portName << " a " << baudRate << " baudios." << std::endl;

    // Esperar a que el Arduino se reinicie (los Arduino se reinician al abrir el puerto)
    Sleep(2000);

    return true;

#else
    // ===== LINUX/macOS IMPLEMENTATION =====
    fd = ::open(portName, O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1) {
        std::cerr << "Error: No se pudo abrir el puerto " << portName << std::endl;
        std::cerr << "Verifica que tienes permisos (sudo) y que el puerto existe." << std::endl;
        return false;
    }

    // Configurar el puerto
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(fd, &tty) != 0) {
        std::cerr << "Error: No se pudo obtener los atributos del puerto" << std::endl;
        ::close(fd);
        fd = -1;
        return false;
    }

    // Configurar velocidad
    speed_t speed;
    switch(baudRate) {
        case 9600: speed = B9600; break;
        case 19200: speed = B19200; break;
        case 38400: speed = B38400; break;
        case 57600: speed = B57600; break;
        case 115200: speed = B115200; break;
        default: speed = B9600; break;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    // Configurar 8N1 (8 bits, sin paridad, 1 bit de parada)
    tty.c_cflag &= ~PARENB; // Sin paridad
    tty.c_cflag &= ~CSTOPB; // 1 bit de parada
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;     // 8 bits por byte
    tty.c_cflag &= ~CRTSCTS; // Sin control de flujo hardware
    tty.c_cflag |= CREAD | CLOCAL; // Activar lectura, ignorar líneas de control

    // Modo no canónico (lectura byte por byte)
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;   // Sin eco
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;   // Sin interpretación de señales

    // Configurar entrada
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Sin control de flujo software
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    // Configurar salida
    tty.c_oflag &= ~OPOST; // Sin procesamiento de salida
    tty.c_oflag &= ~ONLCR;

    // Configurar timeouts
    tty.c_cc[VTIME] = 1;  // Timeout de 0.1 segundos
    tty.c_cc[VMIN] = 0;   // Lectura no bloqueante

    // Aplicar configuración
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cerr << "Error: No se pudo configurar el puerto" << std::endl;
        ::close(fd);
        fd = -1;
        return false;
    }

    connected = true;
    std::cout << "Conexión establecida con " << portName << " a " << baudRate << " baudios." << std::endl;

    // Esperar a que el Arduino se reinicie
    sleep(2);

    return true;
#endif
}

bool SerialPort::readByte(char* buffer) {
    if (!connected) {
        return false;
    }

#ifdef _WIN32
    DWORD bytesRead;
    if (ReadFile(hSerial, buffer, 1, &bytesRead, NULL)) {
        return (bytesRead > 0);
    }
    return false;
#else
    int n = read(fd, buffer, 1);
    return (n > 0);
#endif
}

char* SerialPort::readLine() {
    if (!connected) {
        return nullptr;
    }

    static char lineBuffer[256];
    int linePos = 0;
    char c;

    // Leer hasta encontrar '\n' o '\r'
    while (linePos < 255) {
        if (readByte(&c)) {
            // Ignorar \r y \n al inicio
            if (linePos == 0 && (c == '\n' || c == '\r')) {
                continue;
            }

            // Si encontramos fin de línea, terminar
            if (c == '\n' || c == '\r') {
                break;
            }

            lineBuffer[linePos++] = c;
        } else {
            // No hay más datos disponibles por ahora
            if (linePos > 0) {
                break; // Retornar lo que tenemos
            }
            return nullptr; // No hay datos
        }
    }

    if (linePos == 0) {
        return nullptr; // Línea vacía
    }

    lineBuffer[linePos] = '\0';

    // Alocar memoria y copiar
    char* result = (char*)malloc(linePos + 1);
    if (result) {
        manual_strcpy(result, lineBuffer);
    }

    return result;
}

void SerialPort::close() {
    if (!connected) {
        return;
    }

#ifdef _WIN32
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
#else
    if (fd != -1) {
        ::close(fd);
        fd = -1;
    }
#endif

    connected = false;
    std::cout << "Puerto serial cerrado." << std::endl;
}

bool SerialPort::isConnected() const {
    return connected;
}