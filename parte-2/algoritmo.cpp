#include "algoritmo.hpp"
#include "abierta.hpp"
#include "grafo.hpp"
#include "cerrada.hpp"
#include <algorithm>
#include <limits>
#include <vector>
#include <cmath>
#include <iostream>
#define _USE_MATH_DEFINES

// Constantes para el calculo de la heurística
const double EARTH_R = 6371000.0;
const double GRAD_TO_RAD = M_PI / 180.0; // Lo dejamos precalculado para que vaya más rápido

// Se precalculan los datos del destino para calcular la heurística (ya que como el destino es siempre el mismo, no los tenemos que calcular siempre)
void Algoritmo::inicializar_destino(int nodo_destino, const Grafo &grafo) {
    const auto &n = grafo.get_nodos()[nodo_destino - 1];

    // Convertimos a grados reales
    destino_cache.lat = n.lat / 1e6;
    destino_cache.lon = n.lon / 1e6;
    
    // El coseno de la latitud es necesario para escalar la longitud correctamente
    destino_cache.cos_lat = std::cos(destino_cache.lat * GRAD_TO_RAD);

    destino_inicializado = true;
}


// Dependiendo del valor de usa_h, se ejecuta A* o Dijkstra (heurística == 0)
Resultado Algoritmo::busqueda(int origen, int destino, Grafo &grafo, bool usa_h) {

    // Solamente precalculamos los datos necesarios para calcular la heurística la primera vez que se llama a la función
    if (usa_h && !destino_inicializado) {
        inicializar_destino(destino, grafo);
    }
    // Inicializaciones importantes
    int n = grafo.get_num_nodos();
    Abierta abierta(2 * grafo.get_max_cost() + 1); // En la memoria queda explicado el por que de este módulo
    Cerrada cerrada(n);
    std::vector<int> g_minimos(n, std::numeric_limits<int>::max()); // Se usará para ir actualizando los mejores valores de g
    std::vector<int> padres(n, 0); // Se usa para reconstruir el camino de la solución
    int n_expansiones = 0; // Para mostrarlo como información
    
    // Insertamos el primer nodo en abierta
    int h_ini = 0;
    if (usa_h) {
        h_ini = distancia_euclidea(origen, grafo);
    }
    g_minimos[origen-1] =  0; // Iniciamos el valor de g del origen
    ElementoAbierta e = {origen, h_ini, 0};
    abierta.insertar(e);

    ElementoAbierta actual, hijo;

    while (true) {
        actual = abierta.extraer_minimo();
        if (actual.id == -1) {
            // Lista abierta vacía
            break;
        }

        if (cerrada.contiene(actual.id)) continue; // Si el nodo ya está en cerrada, lo saltamos 
        
        n_expansiones++;
        
        // Meta encontrada
        if (actual.id == destino) {
            // Reconstrucción del camino mediante backtracking (hacia atrás)
            std::vector<std::pair<int, int>> camino; // En el camino guardamos el ID del nodo y el coste de la arista para llegar a él
            for (int v = destino; v != 0; v = padres[v-1]) {
                int p = padres[v-1];
                if (p != 0) {
                    camino.push_back({v, grafo.get_coste_arista(p, v)});
                } else {
                    camino.push_back({v, 0}); // El origen no tiene arista de entrada
                }
            }
            std::reverse(camino.begin(), camino.end());

            return Resultado{actual.g, camino, n_expansiones};
        }

        cerrada.insertar(actual.id);

        for (const auto& arista : grafo.get_nodos()[actual.id-1].vecinos) {
            int v = arista.first;       // ID del vecino
            if (cerrada.contiene(v)) continue; // Si el vecino ya está en cerrada, lo saltamos
            int peso = arista.second;   // Peso de la arista 
            int nuevo_g = actual.g + peso; // Coste acumulado

            // Si ya hemos encontrado un camino mejor, lo saltamos, nos ahorramos meterlo en la lista abierta
            if (nuevo_g < g_minimos[v-1]) {
                g_minimos[v-1] = nuevo_g;
                padres[v-1] = actual.id; // Guardamos el rastro localmente

                int h_v = 0;
                if (usa_h) {
                    h_v = distancia_euclidea(v, grafo);
                }

                hijo = {v, nuevo_g + h_v, nuevo_g};
                abierta.insertar(hijo);
            }
        }
    }
    // Si salimos del bucle sin encontrar el destino
    return Resultado{-1, {}, n_expansiones};
}

// --------------------------------------------------
// Heurística Distancia Euclídea Proyectada
// --------------------------------------------------
inline int Algoritmo::distancia_euclidea(int nodo_origen, const Grafo &grafo) {
    const auto &n = grafo.get_nodos()[nodo_origen - 1];

    double lat = n.lat / 1e6;
    double lon = n.lon / 1e6;

    // Diferencia de latitud y longitud convertida a radianes
    double dLat = (destino_cache.lat - lat) * GRAD_TO_RAD;
    double dLon = (destino_cache.lon - lon) * GRAD_TO_RAD;

    // X es la longitud ajustada por el coseno de la latitud
    double x = dLon * destino_cache.cos_lat;
    double y = dLat;

    // Aplicamos pitagoras para obtener la distancia = R * sqrt(x^2 + y^2)
    return static_cast<int>(EARTH_R * std::sqrt(x * x + y * y)); // Redondeamos para que la heurística devuelva un entero
}