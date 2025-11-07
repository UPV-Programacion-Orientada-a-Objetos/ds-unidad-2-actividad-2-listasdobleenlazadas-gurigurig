/**
* @file TramaLoad.h
 * @brief Define la clase TramaLoad, que representa una trama de carga de datos.
 */

#ifndef TRAMA_LOAD_H
#define TRAMA_LOAD_H

#include "TramaBase.h"
#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"

/**
 * @class TramaLoad
 * @brief Clase que representa una trama de tipo "L" (Load).
 *
 * Estas tramas contienen un fragmento de datos (un carácter) que debe
 * ser decodificado utilizando el estado actual del rotor y luego
 * almacenado en la lista de carga.
 */
class TramaLoad : public TramaBase {
private:
    char data; /**< @brief Carácter de datos que transporta la trama de carga. */

public:
    /**
     * @brief Constructor de TramaLoad.
     * @param data El carácter que esta trama debe cargar y decodificar.
     */
    TramaLoad(char data);

    /**
     * @brief Procesa la trama de carga.
     *
     * Decodifica el carácter de datos utilizando el rotor actual y lo inserta
     * al final de la lista de carga.
     *
     * @param carga Puntero a la ListaDeCarga donde se almacena el carácter decodificado.
     * @param rotor Puntero al RotorDeMapeo
     *              utilizado para decodificar el carácter.
     */
    void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) override;

    /**
     * @brief Destructor de TramaLoad.
     * @details Realiza cualquier limpieza necesaria para la trama de carga.
     */
    ~TramaLoad() override;
};

#endif // TRAMA_LOAD_H