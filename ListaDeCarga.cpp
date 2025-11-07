/**
* @file ListaDeCarga.cpp
 * @brief Implementación de la clase ListaDeCarga.
 */

#include "ListaDeCarga.h"
#include <iostream> // Necesario para std::cout

ListaDeCarga::ListaDeCarga() : head(nullptr), tail(nullptr) {}

ListaDeCarga::~ListaDeCarga() {
    NodoDoble* current = head;
    while (current != nullptr) {
        NodoDoble* next = current->siguiente;
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
}

void ListaDeCarga::insertarAlFinal(char dato) {
    NodoDoble* nuevoNodo = new NodoDoble(dato);
    if (head == nullptr) {
        head = nuevoNodo;
        tail = nuevoNodo;
    } else {
        tail->siguiente = nuevoNodo;
        nuevoNodo->previo = tail;
        tail = nuevoNodo;
    }
}

void ListaDeCarga::imprimirMensaje() {
    NodoDoble* current = head;
    while (current != nullptr) {
        std::cout << current->dato;
        current = current->siguiente;
    }
}