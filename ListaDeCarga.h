/**
* @file ListaDeCarga.h
 * @brief Define una implementación manual de una lista doblemente enlazada para almacenar la carga decodificada.
 */

#ifndef LISTA_DE_CARGA_H
#define LISTA_DE_CARGA_H

/**
 * @struct NodoDoble
 * @brief Estructura de nodo para la lista doblemente enlazada.
 */
struct NodoDoble {
    char dato;          /**< @brief Carácter almacenado en el nodo. */
    NodoDoble* siguiente; /**< @brief Puntero al siguiente nodo en la lista. */
    NodoDoble* previo;    /**< @brief Puntero al nodo anterior en la lista. */

    /**
     * @brief Constructor de NodoDoble.
     * @param d El carácter a almacenar en el nodo.
     */
    NodoDoble(char d) : dato(d), siguiente(nullptr), previo(nullptr) {}
};

/**
 * @class ListaDeCarga
 * @brief Implementación manual de una lista doblemente enlazada.
 *
 * Esta lista se utiliza para almacenar los caracteres decodificados
 * en el orden correcto, formando el mensaje final.
 */
class ListaDeCarga {
private:
    NodoDoble* head; /**< @brief Puntero al primer nodo de la lista. */
    NodoDoble* tail; /**< @brief Puntero al último nodo de la lista. */

public:
    /**
     * @brief Constructor de ListaDeCarga.
     * Inicializa la lista como vacía.
     */
    ListaDeCarga();

    /**
     * @brief Destructor de ListaDeCarga.
     * Libera toda la memoria asignada dinámicamente para los nodos de la lista.
     */
    ~ListaDeCarga();

    /**
     * @brief Inserta un carácter al final de la lista.
     * @param dato El carácter a insertar.
     */
    void insertarAlFinal(char dato);

    /**
     * @brief Imprime el mensaje ensamblado contenido en la lista.
     * Los caracteres se imprimen en el orden de la lista.
     */
    void imprimirMensaje();
};

#endif // LISTA_DE_CARGA_H