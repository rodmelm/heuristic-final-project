#ifndef ALGORITMO_HPP
#define ALGORITMO_HPP

#include "grafo.hpp"
#include <vector>

// Resultado de la búsqueda
struct Resultado {
    int coste_total;
    std::vector<std::pair<int, int>> camino;
    int nodos_expandidos;
};

// Estructura para guardar lo precalculado respecto al destino
struct DestinoCache {
    double lat;
    double lon;
    double cos_lat; 
    
};

class Algoritmo {

public:
    Algoritmo() {}

    Resultado busqueda(int nodo_origen, int nodo_destino, Grafo &grafo, bool heuristica);

private:
    inline int distancia_euclidea(int nodo_origen, const Grafo &grafo);
    void inicializar_destino(int nodo_destino, const Grafo &grafo);
    DestinoCache destino_cache;
    bool destino_inicializado = false;
};

#endif
