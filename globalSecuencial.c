#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void merge(int arr[], int left, int mid, int right)
{
    int i = left, j = mid + 1, k = 0;
    int *temp = (int *)malloc((right - left + 1) * sizeof(int));
    if (temp == NULL)
    {
        printf("Error al asignar memoria.\n");
        return;
    }
    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])
        {
            temp[k++] = arr[i++];
        }
        else
        {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= right)
        temp[k++] = arr[j++];

    for (i = left, k = 0; i <= right; i++, k++)
    {
        arr[i] = temp[k];
    }
    free(temp);
}

void mergeSort(int arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}
int esPrimo(int n)
{
    if (n <= 1)
        return 0;
    if (n <= 3)
        return 1;
    if (n % 2 == 0 || n % 3 == 0)
        return 0;
    int i;
    for (i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    }
    return 1;
}

int contarPrimos(int arr[], int size)
{
    int contador = 0;
    int i;
    for (i = 0; i < size; i++)
    {
        if (esPrimo(arr[i]))
        {
            contador++;
        }
    }
    return contador;
}

int leerArchivo(const char *filename, int **arr)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error al abrir el archivo.\n");
        return 0;
    }
    int maxSize = 1000;
    int count = 0;
    *arr = malloc(maxSize * sizeof(int));
    if (*arr == NULL)
    {
        printf("Error al asignar memoria.\n");
        fclose(file);
        return 0;
    }
    while (fscanf(file, "%d", &(*arr)[count]) == 1)
    {
        count++;
        if (count >= maxSize)
        {
            maxSize *= 2;
            *arr = realloc(*arr, maxSize * sizeof(int));
            if (*arr == NULL)
            {
                printf("Error al reasignar memoria.\n");
                fclose(file);
                return 0;
            }
        }
    }
    fclose(file);
    return count;
}
void escribirArchivo(const char *filename, int arr[], int size)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error al crear el archivo.\n");
        return;
    }
    int i;
    for (i = 0; i < size; i++)
    {
        fprintf(file, "%d ", arr[i]);
    }
    fclose(file);
}
int main(int argc, char *argv[])
{

    MPI_Init(&argc, &argv);
    double inicio = MPI_Wtime();
    printf("La variable tiempoInicio posee el valor %f \n", inicio);

    int *arr;
    int n = leerArchivo("desordenados.txt", &arr);
    if (n == 0)
    {
        printf("No se pudieron leer números del archivo.\n");
        return 1;
    }

    mergeSort(arr, 0, n - 1);
    escribirArchivo("ordenados.txt", arr, n);

    int cantidadPrimos = contarPrimos(arr, n);
    printf("Cantidad de números primos: %d\n", cantidadPrimos);
    double fin = MPI_Wtime();
    printf("Tiempo de ejecución: %f segundos\n", fin - inicio);
    free(arr);

    MPI_Finalize();
    return 0;
}