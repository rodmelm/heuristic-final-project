#include "abierta.hpp"

void Abierta::insertar(ElementoAbierta elemento) {
    // Se encarga de insertar un elemento en la lista abierta (debe insertarlo ordenado)
    if (elemento.f < current_min || current_min == -1) {
        // Si el elemento es menor que el mínimo actual, actualizamos el mínimo
        current_min = elemento.f;
    }
    
    // Obtenemos el índice del bucket y lo insertamos
    int idx = elemento.f % modulo;
    buckets[idx].push_back(elemento);
    num_elementos++;
}

ElementoAbierta Abierta::extraer_minimo() {
    // Lista abierta vacía
    if (num_elementos == 0) {
        return {-1, -1, -1};
    }
    // Calculamos el índice del bucket con el mínimo
    int idx = current_min % modulo;

    // Si resulta que el bucker está vacío, buscamos el siguiente con contenido (este será en nuevo mínimo)
    while (buckets[idx].empty()) {
        current_min++;
        idx = current_min % modulo;
    }

    // Una vez tenemos el bucket con el mínimo, extraemos el primer elemento
    ElementoAbierta res = buckets[idx].back();
    buckets[idx].pop_back();
    num_elementos--;
    
    return res;
}

