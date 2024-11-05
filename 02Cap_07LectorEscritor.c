#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TRUE 1 // Para el ciclo infinito
#define NUM_LECTORES 5 // Número de lectores
#define NUM_ESCRITORES 2 // Número de escritores

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Controla acceso al contador de lectores
pthread_mutex_t bd_mutex = PTHREAD_MUTEX_INITIALIZER; // Controla acceso a la base de datos
int cl = 0; // Contador de lectores activos

void *lector(void *arg) {
    while(TRUE) {
        pthread_mutex_lock(&mutex); // Acceso exclusivo al contador de lectores
        cl++; // Incrementa el número de lectores
        if (cl == 1) {
            pthread_mutex_lock(&bd_mutex); // Si es el primer lector, bloquea acceso a la base de datos
            printf("Lector %ld ha bloqueado la base de datos para escritores.\n", (long)arg);
        }
        pthread_mutex_unlock(&mutex); // Libera acceso al contador de lectores

        // Simulación de lectura
        printf("Lector %ld está leyendo la base de datos.\n", (long)arg);
        sleep(1); // Tiempo de lectura (1 segundo)

        pthread_mutex_lock(&mutex); // Acceso exclusivo al contador de lectores
        cl--; // Decrementa el número de lectores
        if (cl == 0) {
            pthread_mutex_unlock(&bd_mutex); // Si es el último lector, libera acceso a la base de datos
            printf("Lector %ld ha liberado la base de datos para escritores.\n", (long)arg);
        }
        pthread_mutex_unlock(&mutex); // Libera acceso al contador de lectores

        // Simulación de tiempo de espera entre lecturas
        sleep(2); // Tiempo de espera entre lecturas (2 segundos)
    }
    return NULL;
}

void *escritor(void *arg) {
    while(TRUE) {
        printf("Escritor %ld está pensando antes de escribir.\n", (long)arg);
        sleep(3); // Tiempo de pensamiento antes de escribir (3 segundos)

        pthread_mutex_lock(&bd_mutex); // Obtiene acceso exclusivo a la base de datos
        printf("Escritor %ld está escribiendo en la base de datos.\n", (long)arg);
        sleep(2); // Tiempo de escritura (2 segundos)
        pthread_mutex_unlock(&bd_mutex); // Libera acceso a la base de datos

        printf("Escritor %ld ha terminado de escribir.\n", (long)arg);

        // Simulación de tiempo de espera entre escrituras
        sleep(3); // Tiempo de espera entre escrituras (3 segundos)
    }
    return NULL;
}

int main() {
    pthread_t lectores[NUM_LECTORES], escritores[NUM_ESCRITORES];

    for (long i = 0; i < NUM_LECTORES; i++) {
        pthread_create(&lectores[i], NULL, lector, (void *)i);
    }

    for (long i = 0; i < NUM_ESCRITORES; i++) {
        pthread_create(&escritores[i], NULL, escritor, (void *)i);
    }

    for (int i = 0; i < NUM_LECTORES; i++) {
        pthread_join(lectores[i], NULL);
    }

    for (int i = 0; i < NUM_ESCRITORES; i++) {
        pthread_join(escritores[i], NULL);
    }

    return 0;
}
