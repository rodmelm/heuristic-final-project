#include "grafo.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void Grafo::generar_grafo(const std::string& coordenadas, const std::string& distancias) {
    // Genera el grafo en base a los archivos de coordenadas y distancias

    // Los abrimos
    std::ifstream co_file(coordenadas);
    std::ifstream gr_file(distancias);

    // Comprobamos que se han abierto correctamente
    if (!co_file.is_open()) {
        std::cerr << "No se pudo abrir el archivo de coordenadas: " << coordenadas << "\n";
        return;
    }
    if (!gr_file.is_open()) {
        std::cerr << "No se pudo abrir el archivo de distancias: " << distancias << "\n";
        return;
    }

    std::string line;
    //Leemos los archivos para asignar los valores al grafo (vamos a leer línea por línea)
    while (std::getline(gr_file, line)) {
        if (line.empty() || line[0] == 'c') continue; // Lineas que ignoramos

        if (line[0] == 'p') {
            // Aquí se indica el tamaño del grafo
            std::istringstream iss(line);
            char p;
            std::string tipo;
            int num_nodos_file, num_aristas_file;
            iss >> p >> tipo >> num_nodos_file >> num_aristas_file;

            num_nodos = num_nodos_file;
            num_aristas = num_aristas_file;
            nodos.resize(num_nodos);
        }

        if (line[0] == 'a') {
            // Aquí se indica el coste de una arista
            std::istringstream iss(line);
            char a;
            int from, to, cost;
            iss >> a >> from >> to >> cost;

            if (cost > max_cost) {
                max_cost = cost;
            }
            
            nodos[from-1].vecinos.push_back({to, cost});
        }
    }
    // Leemos coordenadas
    while (std::getline(co_file, line)) {
        if (line.empty() || line[0] == 'c' || line[0] == 'p') continue; // Lineas que ignoramos

        if (line[0] == 'v') {
            // Aquí se indica la coordenada de un nodo
            std::istringstream iss(line);
            char v;
            int id, lon, lat;
            iss >> v >> id >> lon >> lat;

            nodos[id-1].lat = lat;
            nodos[id-1].lon = lon;
        }
    }

    co_file.close();
    gr_file.close();
}

int Grafo::get_coste_arista(int u, int v) {
    // Función que devuelve el coste de una arista, la usamos a la hora de reconstruir el camino de la solución
    for (std::pair<int, int> const& arista : nodos[u-1].vecinos) {
        if (arista.first == v) {
            return arista.second;
        }
    }
    return 0;
}