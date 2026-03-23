#ifndef CERRADA_HPP
#define CERRADA_HPP

#include <vector>

// Implementación de la lista cerrada -> lista de booleanos que se van actualizando a True según convenga
class Cerrada {
    public:
        Cerrada(int n_nodos) : cerrada(n_nodos, false) {}   

        void insertar(int nodo) {
            cerrada[nodo-1] = true;
        }

        bool contiene(int nodo) const {
            return cerrada[nodo-1];
        }

    private:
        std::vector<bool> cerrada;
};

#endif