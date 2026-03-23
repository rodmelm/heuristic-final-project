#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <utility>
#include <string>

// Esta estructura representa un nodo del grafo (contiene toda la información necesaria)
struct Nodo {
    int lat;
    int lon;
    std::vector<std::pair<int,int>> vecinos; // Vector de vecinos (id, coste de la arista hacia ese vecino)
};

class Grafo {
    public:
        Grafo() {}

        void generar_grafo(const std::string& coordenadas, const std::string& distancias); 
        int get_coste_arista(int u, int v);

        // Getters
        int get_num_nodos() const { return num_nodos; }
        int get_num_aristas() const { return num_aristas; }
        int get_max_cost() const { return max_cost; }
        const std::vector<Nodo>& get_nodos() const { return nodos; } 
        
    private:
        std::vector<Nodo> nodos; // Vector de nodos
        int num_nodos; // Número de nodos
        int num_aristas; // Número de aristas
        int max_cost = -1; // Coste máximo de una arista -> lo usaremos para calcular el modulo de la lista abierta
};

#endif
