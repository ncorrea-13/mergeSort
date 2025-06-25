# Proyecto Merge Sort Paralelo y Secuencial

Este proyecto implementa algoritmos de ordenación utilizando Merge Sort en versiones paralela y secuencial, además de herramientas para generar números aleatorios.

## Archivos del proyecto

### 1. `creadorNum2.c`
Este archivo genera 50,000,000 números aleatorios en el rango de 0 a 100,000 y los guarda en el archivo `cincoMillones.txt`. Utiliza la función `rand()` para generar los números y los escribe en el archivo secuencialmente.

### 2. `globalParaleloFinal.c`
Implementa una versión paralela del algoritmo Merge Sort utilizando MPI (Message Passing Interface). Este archivo incluye las funciones:
- `merge`: Combina dos subarreglos ordenados.
- `mergeSort`: Aplica recursivamente el algoritmo Merge Sort.
- `esPrimo`: Determina si un número es primo.
- `contarPrimos`: Cuenta la cantidad de números primos en un arreglo.
- `leerArchivo`: Lee números desde un archivo.
- `escribirArchivo`: Escribe números en un archivo.

El programa distribuye los datos entre procesos MPI, realiza la ordenación y el conteo de números primos, y escribe los resultados en `ordenados.txt`.

### 3. `globalSecuencial.c`
Este archivo implementa la versión secuencial del algoritmo Merge Sort. Incluye las mismas funciones que la versión paralela, pero ejecuta todo el proceso en un solo hilo sin distribuir los datos.

### 4. `creadorNumeros.c`
Genera 10,000,000 números aleatorios utilizando la función `rand()` y los guarda en el archivo `diezMillon.txt`. Este archivo es útil para crear conjuntos de datos desordenados para los algoritmos de ordenación.

## Requisitos

1. **Compilador C:** como `gcc`.
2. **MPI:** para ejecutar la versión paralela.
3. **Archivos de entrada:**
   - `desordenados.txt`: Archivo que contiene números desordenados para ser ordenados.

## Cómo ejecutar

### Compilación

Para compilar los archivos, use los siguientes comandos:

```bash
# Para crear números aleatorios
gcc -o creadorNum2 creadorNum2.c

# Para el algoritmo paralelo
mpicc -o globalParaleloFinal globalParaleloFinal.c

# Para el algoritmo secuencial
mpicc -o globalSecuencial globalSecuencial.c

# Para crear otro conjunto de números
gcc -o creadorNumeros creadorNumeros.c
```

### Ejecución

#### Generación de números

Ejecute el generador de números:
```bash
./creadorNum2
```

#### Algoritmo paralelo

Ejecute el programa paralelo con MPI:
```bash
mpirun -np <número_de_procesos> ./globalParaleloFinal
```

#### Algoritmo secuencial

Ejecute el programa secuencial:
```bash
./globalSecuencial
```

#### Generación de otro conjunto de números

Ejecute el generador:
```bash
./creadorNumeros
```

## Notas

- **Archivos generados:**
  - `cincoMillones.txt`: Contiene 50,000,000 números aleatorios.
  - `diezMillon.txt`: Contiene 10,000,000 números aleatorios.
  - `desordenados.txt`: Archivo de entrada para ordenación.
  - `ordenados.txt`: Resultado de la ordenación.

- **MPI:** Asegúrese de tener instalado MPI para ejecutar programas paralelos.

## Autor
Este proyecto fue desarrollado como parte de un estudio sobre algoritmos de ordenación y paralelismo en C.

