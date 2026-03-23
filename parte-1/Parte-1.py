#!/usr/bin/env python3
import sys
import constraint

def leer_fichero(ruta):
    """
    Lee el fichero de entrada y devuelve la matriz del tablero y el tamaño n.
    """
    try:
        with open(ruta, "r") as f:
            # Leemos todas las líneas y eliminamos espacios en blanco extra
            lineas = []
            for linea in f.readlines():
                if linea.strip():
                    lineas.append(linea.strip())
        
        n = len(lineas)
        tablero = []
        # Comprobamos que el tablero es cuadrado
        for fila in lineas:
            if len(fila) != n:
                raise ValueError("El tablero no es cuadrado.")
            tablero.append(list(fila))
        return tablero, n
    except FileNotFoundError:
        print(f"Error: No se encontró el archivo {ruta}")
        sys.exit(1)

def escribir_tablero(tablero, f_out=None):
    """
    Escribe el tablero formateado en el flujo de salida indicado (stdout o fichero).
    Convierte los valores numéricos del modelo (0, 1) a caracteres (O, X).
    """
    n = len(tablero)
    output = []
    # Borde superior
    output.append("+" + "---+" * n)
    for fila in tablero:
        # Convertir '.' a espacio y formatear con espacios alrededor
        fila_escrita = []
        for c in fila:
            if c == ".":
                fila_escrita.append(" ")
            else:
                fila_escrita.append(c)
        output.append("| " + " | ".join(fila_escrita) + " |")
    # Borde inferior
    output.append("+" + "---+" * n)
    
    texto = "\n".join(output)
    
    return texto

def restriccion_no_tres_consecutivos(a, b, c):
    """Restricción maximo 2 consecutivas"""
    return not (a == b == c)

def solver(tablero_inicial, n):
    """
    solver del problema
    """
    problem = constraint.Problem()

    # Definición de Variables y Dominios
    for i in range(n):
        for j in range(n):
            valor = tablero_inicial[i][j]
            if valor.upper() == "X":
                problem.addVariable("V_" + str(i) + "_" + str(j), [1]) # Dominio fijo negro
            elif valor.upper() == "O":
                problem.addVariable("V_" + str(i) + "_" + str(j), [0]) # Dominio fijo blanco
            else:
                problem.addVariable("V_" + str(i) + "_" + str(j), [0, 1]) # Dominio completo

    # Restriccion 1: Suma de filas y columnas debe ser n/2
    objetivo_suma = n // 2
    
    for i in range(n):
        vars_fila = ["V_" + str(i) + "_" + str(j) for j in range(n)]
        problem.addConstraint(lambda *args: sum(args) == objetivo_suma, vars_fila)

    for j in range(n):
        vars_col = ["V_" + str(i) + "_" + str(j) for i in range(n)]
        problem.addConstraint(lambda *args: sum(args) == objetivo_suma, vars_col)

    # Restriccion 2: Máximo 2 consecutivos
    # Comprobamos filas
    for i in range(n):
        for j in range(n - 2):
            problem.addConstraint(restriccion_no_tres_consecutivos, ["V_" + str(i) + "_" + str(j), 
                                                                    "V_" + str(i) + "_" + str(j+1), 
                                                                    "V_" + str(i) + "_" + str(j+2)])
            
    # Comprobamos columnas
    for j in range(n):
        for i in range(n - 2):
            problem.addConstraint(restriccion_no_tres_consecutivos, ["V_" + str(i) + "_" + str(j), 
                                                                    "V_" + str(i+1) + "_" + str(j), 
                                                                    "V_" + str(i+2) + "_" + str(j)])

    return problem.getSolutions()

def main():
    # Validamos argumentos
    if len(sys.argv) != 3:
        print("Error, número de argumentos incorrecto")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]
    tablero_inicial, n = leer_fichero(input_file)
    tablero = escribir_tablero(tablero_inicial)
    print(tablero)
    
    soluciones = solver(tablero_inicial, n)
    num_soluciones = len(soluciones)

    # Mostramos número de soluciones en pantalla
    print(f"\n{num_soluciones} soluciones encontradas")

    # Escribimos en fichero de salida
    with open(output_file, 'w') as f:
        # Primero la instancia original
        f.write(tablero + "\n")
        
        # Luego la primera solución (si existe)
        if num_soluciones > 0:
            sol = soluciones[0]
            # Convertimos el diccionario de solución a matriz con símbolos O/X
            simbolos = {0: "O", 1: "X"}
            matriz_solucion = [['']*n for _ in range(n)]
            for var_name, valor in sol.items():
                # Extraemos i y j
                partes = var_name.split("_")
                i = int(partes[1])
                j = int(partes[2])
                matriz_solucion[i][j] = simbolos[valor]
            
            tablero_solucion = escribir_tablero(matriz_solucion)
            f.write(tablero_solucion)
        else:
            f.write("No se encontraron soluciones.\n")

if __name__ == "__main__":
    main()