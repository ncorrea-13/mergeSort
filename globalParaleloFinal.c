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
    int rank, size;
    MPI_Init(&argc, &argv);
    double tiempoInicio = MPI_Wtime();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        printf("La variable tiempoInicio posee el valor %f \n", tiempoInicio);
    }

    int *arr = NULL;
    int n;
    int cantidadPrimos;

    if (rank == 0)
    {
        n = leerArchivo("desordenados.txt", &arr);
        if (n == 0)
        {
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_n = n / size + (rank < n % size);
    int *local_arr = (int *)malloc(local_n * sizeof(int));
    if (local_arr == NULL)
    {
        printf("Error al asignar memoria para el arreglo local.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int *sendcounts = (int *)malloc(size * sizeof(int));
    int *displs = (int *)malloc(size * sizeof(int));
    if (sendcounts == NULL || displs == NULL)
    {
        printf("Error al asignar memoria para sendcounts o displs.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int i;
    for (i = 0; i < size; i++)
    {
        sendcounts[i] = n / size + (i < n % size);
        displs[i] = (i > 0) ? displs[i - 1] + sendcounts[i - 1] : 0;
    }

    MPI_Scatterv(arr, sendcounts, displs, MPI_INT, local_arr, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    mergeSort(local_arr, 0, local_n - 1);

    int local_count = contarPrimos(local_arr, local_n);
    MPI_Reduce(&local_count, &cantidadPrimos, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    int *temp_arr = NULL;
    if (rank == 0)
    {
        temp_arr = (int *)malloc(n * sizeof(int));
        MPI_Gatherv(local_arr, local_n, MPI_INT, temp_arr, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
        mergeSort(temp_arr, 0, n - 1);
        escribirArchivo("ordenados.txt", temp_arr, n);
        printf("Cantidad de números primos: %d\n", cantidadPrimos);

        double fin = MPI_Wtime();

        // double tiempoPasado = (fin-tiempoInicio);
        printf("tiempo de ejecucion %f segundos\n", (fin - tiempoInicio));
        printf("tiempo final es de %f segundos\n", fin);

        free(temp_arr);
    }
    else
    {
        MPI_Gatherv(local_arr, local_n, MPI_INT, NULL, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD);
    }

    free(local_arr);
    free(sendcounts);
    free(displs);
    if (rank == 0)
    {
        free(arr);
    }

    MPI_Finalize();
    return 0;
}