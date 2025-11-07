/**
 * @file RotorDeMapeo.cpp
 * @brief Implementación de la clase RotorDeMapeo.
 */

#include "RotorDeMapeo.h"
#include <cctype>   // Necesario para toupper

RotorDeMapeo::RotorDeMapeo() : cabeza(nullptr) {
    inicializarAlfabeto();
}

RotorDeMapeo::~RotorDeMapeo() {
    if (cabeza == nullptr) {
        return;
    }

    NodoCircular* current = cabeza->siguiente;
    while (current != cabeza) {
        NodoCircular* next = current->siguiente;
        delete current;
        current = next;
    }
    delete cabeza; // Eliminar el último nodo restante (la cabeza)
    cabeza = nullptr;
}

void RotorDeMapeo::inicializarAlfabeto() {
    if (cabeza != nullptr) {
        // La lista ya está inicializada, no hacer nada.
        return;
    }

    NodoCircular* current = nullptr;
    NodoCircular* newNode = nullptr;

    // Añadir A-Z
    for (char c = 'A'; c <= 'Z'; ++c) {
        newNode = new NodoCircular(c);
        if (cabeza == nullptr) {
            cabeza = newNode;
            cabeza->siguiente = cabeza;
            cabeza->previo = cabeza;
            current = cabeza;
        } else {
            current->siguiente = newNode;
            newNode->previo = current;
            newNode->siguiente = cabeza; // Enlazar de vuelta a la cabeza para cerrar el círculo
            cabeza->previo = newNode;   // Enlazar el previo de la cabeza al nuevo nodo
            current = newNode;
        }
    }

    // Añadir el espacio ' '
    newNode = new NodoCircular(' ');
    current->siguiente = newNode;
    newNode->previo = current;
    newNode->siguiente = cabeza;
    cabeza->previo = newNode;
}


void RotorDeMapeo::rotar(int n) {
    if (cabeza == nullptr) {
        return;
    }

    if (n > 0) {
        for (int i = 0; i < n; ++i) {
            cabeza = cabeza->siguiente; // Mover la cabeza hacia adelante (en sentido horario)
        }
    } else { // n <= 0
        for (int i = 0; i > n; --i) { // Iterar abs(n) veces para n negativo
            cabeza = cabeza->previo;    // Mover la cabeza hacia atrás (en sentido anti-horario)
        }
    }
}

char RotorDeMapeo::getMapeo(char in) {
    // Determinar el índice absoluto de 'in' en el alfabeto sin rotar (A=0, B=1, ..., ' '=26)
    int absoluteIndex = -1;
    char upperIn = toupper(in); // Convertir a mayúscula para la verificación A-Z

    if (upperIn >= 'A' && upperIn <= 'Z') {
        absoluteIndex = upperIn - 'A';
    } else if (in == ' ') {
        absoluteIndex = 26; // El espacio se considera después de 'Z'
    } else {
        // Si el carácter no se encuentra en el alfabeto del rotor, devolverlo tal cual.
        return in;
    }

    // Ahora, partiendo de la 'cabeza' actual, avanzar 'absoluteIndex' pasos
    NodoCircular* temp = cabeza;
    for (int i = 0; i < absoluteIndex; ++i) {
        temp = temp->siguiente;
    }
    return temp->dato; // Este es el carácter mapeado
}