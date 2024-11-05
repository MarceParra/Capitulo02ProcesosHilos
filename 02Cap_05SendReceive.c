#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_ESTUDIANTES 5 // Número de estudiantes
#define MAX_LIBROS 5      // Número total de libros a enviar

// Estructura del libro
typedef struct {
    int id; // ID del libro
} libro;

// Variables globales
pthread_mutex_t mutex;                       // Mutex para sincronización
pthread_cond_t cond_estudiantes[NUM_ESTUDIANTES]; // Condiciones para cada estudiante
libro libros[MAX_LIBROS];                    // Libros que se van a enviar
int libros_disponibles = 0;                   // Contador de libros disponibles
int libro_actual = 0;                         // Índice del libro actual a enviar

// Función del profesor
void *profesor(void *arg) {
    for (int i = 0; i < MAX_LIBROS; i++) {
        // Simula la creación de un libro
        libro nuevo_libro;
        nuevo_libro.id = i + 1; // Libros de 1 a 5

        // Enviar el libro al estudiante correspondiente
        pthread_mutex_lock(&mutex);
        libros[libros_disponibles] = nuevo_libro; // Agregar libro al arreglo
        libros_disponibles++;
        printf("Profesor: enviando el libro %d\n", nuevo_libro.id);
        
        // Señalar al estudiante correspondiente
        pthread_cond_signal(&cond_estudiantes[i]);
        pthread_mutex_unlock(&mutex);

        sleep(1); // Simula el tiempo para enviar el libro
    }
    return NULL;
}

// Función de cada estudiante
void *estudiante(void *arg) {
    int id_estudiante = *((int *)arg); // Obtener el ID del estudiante
    for (int i = 0; i < MAX_LIBROS; i++) {
        pthread_mutex_lock(&mutex);

        // Esperar a recibir el libro
        while (libros_disponibles <= i) {
            pthread_cond_wait(&cond_estudiantes[id_estudiante], &mutex);
        }

        // Recibir el libro correspondiente
        libro recibido = libros[i];
        libros_disponibles--; // Reducir el contador de libros disponibles
        printf("Estudiante %d: recibió el libro %d\n", id_estudiante + 1, recibido.id);
        pthread_mutex_unlock(&mutex);
        
        sleep(1); // Simula el tiempo que toma al estudiante leer el libro
    }
    return NULL;
}

int main() {
    pthread_t prof; // Hilo del profesor
    pthread_t est[NUM_ESTUDIANTES]; // Hilos de estudiantes
    int ids[NUM_ESTUDIANTES]; // IDs de estudiantes

    pthread_mutex_init(&mutex, NULL); // Inicializa el mutex

    // Inicializa las condiciones
    for (int i = 0; i < NUM_ESTUDIANTES; i++) {
        pthread_cond_init(&cond_estudiantes[i], NULL);
        ids[i] = i; // Asigna ID a cada estudiante
    }

    // Crea el hilo del profesor
    pthread_create(&prof, NULL, profesor, NULL);

    // Crea los hilos de los estudiantes
    for (int i = 0; i < NUM_ESTUDIANTES; i++) {
        pthread_create(&est[i], NULL, estudiante, &ids[i]);
    }

    // Espera a que el profesor termine
    pthread_join(prof, NULL);

    // Espera a que todos los estudiantes terminen
    for (int i = 0; i < NUM_ESTUDIANTES; i++) {
        pthread_join(est[i], NULL);
    }

    // Destruye el mutex y las condiciones
    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < NUM_ESTUDIANTES; i++) {
        pthread_cond_destroy(&cond_estudiantes[i]);
    }

    return 0;
}
