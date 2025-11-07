/**
* @file TramaLoad.cpp
 * @brief Implementación de la clase TramaLoad.
 */

#include "TramaLoad.h"

TramaLoad::TramaLoad(char data) : data(data) {
    // Constructor de TramaLoad
}

void TramaLoad::procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) {
    if (!carga || !rotor) {
        // Manejo de errores básico si los punteros son nulos.
        // En un sistema real, esto podría lanzar una excepción o registrar un error.
        return;
    }

    char decodedChar = rotor->getMapeo(this->data);
    carga->insertarAlFinal(decodedChar);
}

TramaLoad::~TramaLoad() {
    // Destructor de TramaLoad.
    // No se requiere limpieza explícita ya que 'data' es un miembro simple.
}