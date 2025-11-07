/**
* @file TramaMap.h
 * @brief Define la clase TramaMap, que representa una trama de mapeo del rotor.
 */

#ifndef TRAMA_MAP_H
#define TRAMA_MAP_H

#include "TramaBase.h"
#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"

/**
 * @class TramaMap
 * @brief Clase que representa una trama de tipo "M" (Map).
 *
 * Estas tramas contienen una cantidad entera que indica cuánto debe
 * rotar el rotor de mapeo.
 */
class TramaMap : public TramaBase {
private:
    int rotationAmount; /**< @brief Cantidad de rotación para el rotor (positivo o negativo). */

public:
    /**
     * @brief Constructor de TramaMap.
     * @param rotationAmount El valor entero que indica la cantidad y dirección de la rotación.
     */
    TramaMap(int rotationAmount);

    /**
     * @brief Procesa la trama de mapeo.
     *
     * Aplica la rotación especificada por `rotationAmount` al rotor.
     *
     * @param carga Puntero a la ListaDeCarga (no utilizado por TramaMap, pero requerido por la interfaz base).
     * @param rotor Puntero al RotorDeMapeo
     *              sobre el cual se realiza la rotación.
     */
    void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) override;

    /**
     * @brief Destructor de TramaMap.
     * @details Realiza cualquier limpieza necesaria para la trama de mapeo.
     */
    ~TramaMap() override;
};

#endif // TRAMA_MAP_H