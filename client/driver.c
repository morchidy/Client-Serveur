/* driver.c */ 

#include "CL_include"
#include <unistd.h>

extern int pipefd_driver[2];  // Pipe entre Rédacteur et Driver

// Fonction principale du Driver pour afficher les données reçues
void afficher_donnees() {
    int data[5];  // Buffer pour recevoir 5 données du Rédacteur

    while (1) {
        // Lecture de 5 données depuis le pipe du Rédacteur
        for (int i = 0; i < 5; i++) {
            if (read(pipefd_driver[0], &data[i], sizeof(int)) == -1) {
                perror("Erreur de lecture dans le pipe (Driver)");
                exit(1);
            }
        }

        // Affichage des 5 données reçues
        printf("Driver : Affichage des 5 données : ");
        for (int i = 0; i < 5; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }
}
