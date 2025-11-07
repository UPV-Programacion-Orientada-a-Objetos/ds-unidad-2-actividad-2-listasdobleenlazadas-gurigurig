/**
 * @file RotorDeMapeo.h
 * @brief Define una implementación manual de una lista circular doblemente enlazada (RotorDeMapeo).
 */

#ifndef ROTOR_DE_MAPEO_H
#define ROTOR_DE_MAPEO_H

/**
 * @struct NodoCircular
 * @brief Estructura de nodo para la lista circular doblemente enlazada.
 */
struct NodoCircular {
    char dato;           /**< @brief Carácter almacenado en el nodo (e.g., 'A', 'B', ' '). */
    NodoCircular* siguiente; /**< @brief Puntero al siguiente nodo en la lista circular. */
    NodoCircular* previo;    /**< @brief Puntero al nodo anterior en la lista circular. */

    /**
     * @brief Constructor de NodoCircular.
     * @param d El carácter a almacenar en el nodo.
     */
    NodoCircular(char d) : dato(d), siguiente(nullptr), previo(nullptr) {}
};

/**
 * @class RotorDeMapeo
 * @brief Implementación manual de una lista circular doblemente enlazada que actúa
 *        como un "disco de cifrado" o "RotorDeMapeo".
 *
 * Contiene el alfabeto (A-Z y espacio) y puede rotar, cambiando el mapeo
 * de los caracteres. El puntero `cabeza` indica la posición 'cero' actual.
 */
class RotorDeMapeo {
private:
    NodoCircular* cabeza; /**< @brief Puntero a la 'cabeza' de la lista, que indica la posición 'cero' actual. */

public:
    /**
     * @brief Constructor de RotorDeMapeo.
     * Inicializa la lista y carga el alfabeto (A-Z y espacio).
     */
    RotorDeMapeo();

    /**
     * @brief Destructor de RotorDeMapeo.
     * Libera toda la memoria asignada dinámicamente para los nodos de la lista.
     */
    ~RotorDeMapeo();

    /**
     * @brief Inicializa la lista circular con los caracteres del alfabeto (A-Z) y el espacio.
     * Se llama automáticamente desde el constructor.
     */
    void inicializarAlfabeto();

    /**
     * @brief Rota la 'cabeza' de la lista circular N posiciones.
     *
     * Una rotación positiva (N > 0) mueve la cabeza hacia adelante (siguiente).
     * Una rotación negativa (N < 0) mueve la cabeza hacia atrás (previo).
     *
     * @param n El número de posiciones a rotar y la dirección.
     */
    void rotar(int n);

    /**
     * @brief Realiza el mapeo de un carácter de entrada según la rotación actual del rotor.
     *
     * Busca el índice absoluto del carácter de entrada en un alfabeto sin rotar (A=0, B=1, ... Espacio=26).
     * Luego, avanza ese mismo número de pasos desde la `cabeza` actual del rotor para encontrar
     * el carácter mapeado.
     *
     * @param in El carácter de entrada a mapear.
     * @return El carácter mapeado según la configuración actual del rotor.
     *         Si el carácter no se encuentra en el alfabeto del rotor, se devuelve el carácter original.
     */
    char getMapeo(char in);
};

#endif // ROTOR_DE_MAPEO_H