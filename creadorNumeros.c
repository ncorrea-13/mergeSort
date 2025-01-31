#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_NUMEROS 10000000

int main()
{

        srand(time(NULL));
        FILE *file = fopen("diezMillon.txt", "w");
        if (file == NULL)
        {
                perror("Error al abrir el archivo");
                return 1;
        }
        int i;
        for (i = 0; i < NUM_NUMEROS; i++)
        {
                int numero_aleatorio = rand();
                fprintf(file, "%d\n", numero_aleatorio);
        }

        fclose(file);
        printf("Se han generado %d números aleatorios y se han guardado en desordenados.txt\n", NUM_NUMEROS);
        return 0;
}