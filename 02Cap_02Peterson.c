#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define FALSE 0
#define TRUE 1
#define N 2  // Número de procesos

int turno;               // Variable que indica de quién es el turno
int interesado[N];       // Array que indica si un proceso está interesado en entrar a la región crítica

// Función que permite a un proceso entrar en la región crítica
void entrar_region(int proceso) {
    int otro = 1 - proceso; // Determina el otro proceso

    interesado[proceso] = TRUE; // Indica que el proceso quiere entrar
    turno = proceso; // Establece el turno al proceso actual

    // Esperar hasta que sea seguro entrar
    while (turno == proceso && interesado[otro] == TRUE) {
        // Esperar activamente
    }
}

// Función que permite a un proceso salir de la región crítica
void salir_region(int proceso) {
    interesado[proceso] = FALSE; // Indica que ha salido
}

// Simulación de procesos
void* proceso_funcion(void* arg) {
    int proceso = *(int*)arg; // Identificar el proceso (0 o 1)

    for (int i = 0; i < 5; i++) { // Intentar entrar 5 veces
        entrar_region(proceso); // Entrar a la región crítica

        // Sección crítica
        printf("Proceso %d está en la sección crítica.\n", proceso);
        sleep(1); // Simula el tiempo que pasa en la sección crítica

        salir_region(proceso); // Salir de la región crítica
        printf("Proceso %d ha salido de la sección crítica.\n", proceso);
        
        sleep(1); // Simula tiempo de espera antes de intentar de nuevo
    }
    
    return NULL;
}

int main() {
    pthread_t hilos[N]; // Array para almacenar los identificadores de los hilos
    int indices[N] = {0, 1}; // Identificadores de los procesos

    // Inicializar el array interesado a FALSE
    for (int i = 0; i < N; i++) {
        interesado[i] = FALSE; // Ningún proceso está interesado al inicio
    }

    // Crear hilos
    for (int i = 0; i < N; i++) {
        pthread_create(&hilos[i], NULL, proceso_funcion, (void*)&indices[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < N; i++) {
        pthread_join(hilos[i], NULL);
    }

    return 0; // Retorna 0 para indicar que el programa terminó correctamente
}
