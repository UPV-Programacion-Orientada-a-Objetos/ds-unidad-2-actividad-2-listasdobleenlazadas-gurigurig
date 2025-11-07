/**
 * @file main.cpp
 * @brief Programa principal para el decodificador de protocolo industrial PRT-7.
 *
 * Implementa la lógica principal de lectura de tramas desde un puerto serial REAL,
 * parseo, procesamiento polimórfico y ensamblaje del mensaje oculto.
 */

#include <iostream>
#include <cstdlib>

// Inclusiones de las clases del proyecto
#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"
#include "TramaBase.h"
#include "TramaLoad.h"
#include "TramaMap.h"
#include "SerialPort.h"

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
 * @brief Convierte un entero a una cadena de caracteres.
 */
char* itoa_custom(int val, char* buf) {
    if (buf == nullptr) return nullptr;

    int i = 0;
    int isNegative = 0;

    if (val == 0) {
        buf[i++] = '0';
        buf[i] = '\0';
        return buf;
    }

    if (val < 0) {
        isNegative = 1;
        val = -val;
    }

    int start = 0;
    while (val != 0) {
        buf[i++] = (val % 10) + '0';
        val /= 10;
    }

    if (isNegative) {
        buf[i++] = '-';
    }

    buf[i] = '\0';

    // Invertir la cadena
    int end = i - 1;
    while (start < end) {
        char temp = buf[start];
        buf[start] = buf[end];
        buf[end] = temp;
        start++;
        end--;
    }
    return buf;
}

/**
 * @brief Parsea una línea de texto recibida y crea el objeto TramaBase correspondiente.
 */
TramaBase* parseLine(char* line, char* originalDataBuffer, int* rotationValue) {
    if (line == nullptr || manual_strlen(line) < 3) {
        originalDataBuffer[0] = '\0';
        *rotationValue = 0;
        return nullptr;
    }

    char type = line[0];

    // Verificar que hay una coma en la posición correcta
    if (line[1] != ',') {
        originalDataBuffer[0] = '\0';
        *rotationValue = 0;
        return nullptr;
    }

    char* token = line + 2; // Avanzar 2 posiciones para saltar "X,"

    if (type == 'L') {
        char dataChar = *token;
        // Manejar el caso especial del espacio
        if (dataChar == '\0' || dataChar == ' ') {
            dataChar = ' '; // Asegurar que sea un espacio
        }
        originalDataBuffer[0] = dataChar;
        originalDataBuffer[1] = '\0';
        *rotationValue = 0;
        return new TramaLoad(dataChar);
    } else if (type == 'M') {
        int sign = 1;
        if (*token == '-') {
            sign = -1;
            token++;
        } else if (*token == '+') {
            token++;
        }

        int rot = 0;
        while (*token >= '0' && *token <= '9') {
            rot = rot * 10 + (*token - '0');
            token++;
        }
        *rotationValue = rot * sign;
        originalDataBuffer[0] = '\0';
        return new TramaMap(*rotationValue);
    }
    originalDataBuffer[0] = '\0';
    *rotationValue = 0;
    return nullptr;
}

/**
 * @brief Función principal del programa.
 */
int main() {
    std::cout << "==================================================" << std::endl;
    std::cout << "           DECODIFICADOR DE PROTOCOLO             " << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << std::endl;

    // Solicitar el puerto COM al usuario
    std::cout << "Ingrese el nombre del puerto serial:" << std::endl;
    std::cout << "  - Windows: COM3, COM4, etc." << std::endl;
    std::cout << "  - Linux: /dev/ttyUSB0, /dev/ttyACM0, etc." << std::endl;
    std::cout << "  - macOS: /dev/tty.usbserial-*, /dev/tty.usbmodem*, etc." << std::endl;
    std::cout << std::endl;
    std::cout << "Puerto: ";

    char portName[50];
    std::cin.getline(portName, 50);

    std::cout << std::endl;
    std::cout << "Iniciando Decodificador PRT-7. Conectando a puerto " << portName << "..." << std::endl;

    SerialPort serial;
    if (!serial.open(portName, 9600)) {
        std::cerr << std::endl;
        std::cerr << "ERROR: No se pudo abrir el puerto serial." << std::endl;
        std::cerr << "Verifica que:" << std::endl;
        std::cerr << "  1. El Arduino esté conectado" << std::endl;
        std::cerr << "  2. El nombre del puerto sea correcto" << std::endl;
        std::cerr << "  3. Tengas permisos suficientes (en Linux, agrega tu usuario al grupo 'dialout')" << std::endl;
        return 1;
    }

    std::cout << "Esperando tramas del Arduino..." << std::endl;
    std::cout << std::endl;

    ListaDeCarga miListaDeCarga;
    RotorDeMapeo miRotorDeMapeo;

    char* receivedLine;
    char originalCharBuffer[2];
    int rotationAmount = 0;
    bool running = true;

    std::cout << std::endl;
    std::cout << "Presiona 'Q' + ENTER en cualquier momento para detener el programa." << std::endl;
    std::cout << std::endl;

    while (running) {
        receivedLine = serial.readLine();

        if (receivedLine == nullptr) {
            // No hay datos disponibles, continuar esperando
            continue;
        }

        // Ignorar líneas que no son tramas (mensajes del Arduino)
        if (receivedLine[0] != 'L' && receivedLine[0] != 'M') {
            // Es un mensaje informativo del Arduino, no una trama
            std::cout << "[INFO Arduino]: " << receivedLine << std::endl;
            free(receivedLine);
            continue;
        }

        TramaBase* trama = parseLine(receivedLine, originalCharBuffer, &rotationAmount);

        std::cout << "Trama recibida: [" << receivedLine << "] -> Procesando... -> ";

        if (trama != nullptr) {
            // Usar dynamic_cast para determinar el tipo real de la trama
            if (dynamic_cast<TramaLoad*>(trama) != nullptr) {
                char originalInputChar = originalCharBuffer[0];
                char decodedChar = miRotorDeMapeo.getMapeo(originalInputChar);
                trama->procesar(&miListaDeCarga, &miRotorDeMapeo);

                // Mostrar el carácter de forma legible
                if (originalInputChar == ' ') {
                    std::cout << "Fragmento 'Space' decodificado como '";
                } else {
                    std::cout << "Fragmento '" << originalInputChar << "' decodificado como '";
                }

                if (decodedChar == ' ') {
                    std::cout << "Space";
                } else {
                    std::cout << decodedChar;
                }
                std::cout << "'. ";

            } else if (dynamic_cast<TramaMap*>(trama) != nullptr) {
                trama->procesar(&miListaDeCarga, &miRotorDeMapeo);
                char buffer[10];
                std::cout << "ROTANDO ROTOR " << itoa_custom(rotationAmount, buffer) << ". ";
                std::cout << "(Ahora 'A' se mapea a '" << miRotorDeMapeo.getMapeo('A') << "') ";
            }

            // Imprimir el estado actual del mensaje ensamblado
            std::cout << "Mensaje: [";
            miListaDeCarga.imprimirMensaje();
            std::cout << "]" << std::endl;

            delete trama;
        } else {
            std::cerr << "Error: No se pudo parsear la trama: " << receivedLine << std::endl;
        }

        free(receivedLine);
    }

    std::cout << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Flujo de datos terminado." << std::endl;
    std::cout << "MENSAJE OCULTO ENSAMBLADO:" << std::endl;
    miListaDeCarga.imprimirMensaje();
    std::cout << std::endl;
    std::cout << "Liberando memoria... Sistema apagado." << std::endl;

    return 0;
}