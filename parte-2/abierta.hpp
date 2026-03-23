#ifndef ABIERTA_HPP
#define ABIERTA_HPP

#include <vector>

// Estructura que representa un elemento de la lista abierta
struct ElementoAbierta {
    int id;
    int f; // f = g + h
    int g; // g = coste acumulado
};

// Implementación de la lista abierta usando Dials bucket circular
class Abierta {
    public:
        
        Abierta(int modulo) : modulo(modulo), num_elementos(0), current_min(-1) {
            buckets.resize(modulo);
        }

        void insertar(ElementoAbierta elemento);
        ElementoAbierta extraer_minimo();
        bool empty() const { return num_elementos == 0; }

    private:
        std::vector<std::vector<ElementoAbierta>> buckets; // Cada bucket tiene una lista de elementos (ya que puede ser que haya elementos con el mismo f)
        int modulo;
        int num_elementos;
        int current_min; // El valor mínimo de f en la lista abierta
};

#endif