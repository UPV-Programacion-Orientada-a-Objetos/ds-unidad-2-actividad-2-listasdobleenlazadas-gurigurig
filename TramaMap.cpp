/**
* @file TramaMap.cpp
 * @brief Implementación de la clase TramaMap.
 */

#include "TramaMap.h"

TramaMap::TramaMap(int rotationAmount) : rotationAmount(rotationAmount) {
    // Constructor de TramaMap
}

void TramaMap::procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) {
    // 'carga' no es utilizado por TramaMap, pero es parte de la interfaz base.
    // Se asegura de que el puntero del rotor no sea nulo antes de operar.
    if (!rotor) {
        return;
    }
    rotor->rotar(this->rotationAmount);
}

TramaMap::~TramaMap() {
    // Destructor de TramaMap.
    // No se requiere limpieza explícita ya que 'rotationAmount' es un miembro simple.
}