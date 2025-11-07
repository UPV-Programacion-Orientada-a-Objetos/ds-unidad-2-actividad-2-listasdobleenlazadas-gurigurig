/**
 * @file SerialPort.h
 * @brief Define una clase para la comunicación serial REAL con Arduino.
 */

#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#ifdef _WIN32
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

/**
 * @class SerialPort
 * @brief Clase para comunicación serial real con Arduino.
 *
 * Esta implementación funciona con puertos COM reales en:
 * - Windows: COM1, COM2, COM3, etc.
 * - Linux/macOS: /dev/ttyUSB0, /dev/ttyACM0, etc.
 *
 * @note Configuración por defecto: 9600 baudios, 8N1 (8 bits, sin paridad, 1 bit de parada)
 */
class SerialPort {
private:
#ifdef _WIN32
    HANDLE hSerial;  /**< @brief Handle del puerto serial en Windows. */
#else
    int fd;          /**< @brief File descriptor del puerto serial en Linux/macOS. */
    struct termios tty; /**< @brief Configuración del terminal en Linux/macOS. */
#endif
    bool connected;  /**< @brief Estado de la conexión. */
    char readBuffer[256]; /**< @brief Buffer interno para lectura. */
    int bufferPos;   /**< @brief Posición actual en el buffer. */

public:
    /**
     * @brief Constructor de SerialPort.
     * Inicializa el puerto como no conectado.
     */
    SerialPort();

    /**
     * @brief Destructor de SerialPort.
     * Asegura que el puerto serial se cierre correctamente.
     */
    ~SerialPort();

    /**
     * @brief Abre y configura el puerto serial real.
     * @param portName El nombre del puerto:
     *                 - Windows: "COM3", "COM4", etc.
     *                 - Linux: "/dev/ttyUSB0", "/dev/ttyACM0", etc.
     *                 - macOS: "/dev/tty.usbserial-*", "/dev/tty.usbmodem*", etc.
     * @param baudRate Velocidad de transmisión (por defecto 9600).
     * @return `true` si el puerto se abrió correctamente, `false` en caso contrario.
     */
    bool open(const char* portName, int baudRate = 9600);

    /**
     * @brief Lee una línea de datos del puerto serial.
     * @details
     * La función lee caracteres hasta encontrar un '\n' o '\r'.
     * Elimina los caracteres de nueva línea del final de la cadena.
     * La memoria para la línea leída se asigna dinámicamente y el llamador es
     * responsable de liberarla usando `free()`.
     * @return Un puntero a un array de caracteres `char*` con la línea leída,
     *         o `nullptr` si no hay datos disponibles o hubo un error.
     */
    char* readLine();

    /**
     * @brief Cierra el puerto serial.
     */
    void close();

    /**
     * @brief Verifica si el puerto está conectado.
     * @return `true` si está conectado, `false` en caso contrario.
     */
    bool isConnected() const;

private:
    /**
     * @brief Lee un solo byte del puerto serial.
     * @param buffer Puntero al buffer donde se almacenará el byte leído.
     * @return `true` si se leyó un byte correctamente, `false` en caso contrario.
     */
    bool readByte(char* buffer);
};

#endif // SERIAL_PORT_H