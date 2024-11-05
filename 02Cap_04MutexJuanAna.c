#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_COPIES 5 // Número máximo de copias que cada niño puede hacer

pthread_mutex_t photocopier_mutex; // Mutex para la fotocopiadora

void* child(void* name) {
    char* child_name = (char*) name; // Nombre del niño
    for (int i = 0; i < MAX_COPIES; i++) {
        printf("%s: esperando para usar la fotocopiadora...\n", child_name);
        
        // Intentar usar la fotocopiadora
        pthread_mutex_lock(&photocopier_mutex); // Toma el candado
        printf("%s: usando la fotocopiadora para copia %d\n", child_name, i + 1);
        
        // Simula el tiempo que toma hacer copias
        sleep(1);
        
        printf("%s: terminó la copia %d\n", child_name, i + 1);
        pthread_mutex_unlock(&photocopier_mutex); // Libera el candado
        
        // Simula el tiempo antes de hacer la próxima copia
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t juan, ana; // Dos hilos para Juan y Ana

    pthread_mutex_init(&photocopier_mutex, NULL); // Inicializa el mutex

    // Crea dos hilos (Juan y Ana)
    pthread_create(&juan, NULL, child, "Juan");
    pthread_create(&ana, NULL, child, "Ana");

    // Espera a que ambos hilos terminen
    pthread_join(juan, NULL);
    pthread_join(ana, NULL);

    pthread_mutex_destroy(&photocopier_mutex); // Destruye el mutex

    return 0; // Finaliza el programa
}
