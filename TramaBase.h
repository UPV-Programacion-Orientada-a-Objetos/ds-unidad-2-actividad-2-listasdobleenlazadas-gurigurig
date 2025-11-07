/**
* @file TramaBase.h
 * @brief Define la interfaz base abstracta para todas las tramas de protocolo.
 */

#ifndef TRAMA_BASE_H
#define TRAMA_BASE_H

// Forward declarations para evitar dependencias circulares.
// Permite que TramaBase conozca estas clases sin incluirlas completamente aquí.
class ListaDeCarga;
class RotorDeMapeo;

/**
 * @class TramaBase
 * @brief Clase base abstracta que define la interfaz común para todas las tramas
 *        que llegan del puerto serial.
 *
 * Esta clase asegura que todas las tramas derivadas (TramaLoad, TramaMap)
 * implementen un método `procesar` para gestionar su lógica específica
 * de decodificación o modificación del rotor.
 */
class TramaBase {
public:
    /**
     * @brief Método virtual puro para procesar la trama.
     *
     * Este método debe ser implementado por las clases derivadas para
     * llevar a cabo la lógica específica de la trama.
     *
     * @param carga Puntero a la ListaDeCarga donde se almacenan
     *              los caracteres decodificados.
     * @param rotor Puntero al RotorDeMapeo
     *              que gestiona la rotación y el mapeo de caracteres.
     */
    virtual void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) = 0;

    /**
     * @brief Destructor virtual obligatorio.
     *
     * Asegura la correcta liberación de memoria para objetos polimórficos
     * cuando se eliminan a través de un puntero a TramaBase.
     */
    virtual ~TramaBase() {}
};

#endif // TRAMA_BASE_H