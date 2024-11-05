#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMERO_DE_HILOS 10

void *imprimir_hola_mundo(void *tid) {
    printf("Hola mundo. Saludos del hilo %ld\n", (long)tid);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t hilos[NUMERO_DE_HILOS];
    int estado;
    long i;

    // Crear todos los hilos en un solo bucle
    for(i = 0; i < NUMERO_DE_HILOS; i++) {
        estado = pthread_create(&hilos[i], NULL, imprimir_hola_mundo, (void *)i);
        
        if (estado != 0) {
            printf("Ups. pthread_create devolvió el código de error %d\n", estado);
            exit(-1);
        }
    }

    // Esperar a que todos los hilos terminen
    for(i = 0; i < NUMERO_DE_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    return 0;
} 