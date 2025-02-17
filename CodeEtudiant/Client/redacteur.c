/* redacteur.c */

#include "CL_include"
#include <unistd.h>

extern int pipefd[2];         // Pipe entre Lecteur et Rédacteur
extern int pipefd_driver[2];  // Pipe entre Rédacteur et Driver

// Fonction principale du rédacteur pour recevoir et transmettre les données
void recevoir_donnees() {
    int data[5];  // Buffer pour accumuler 5 données avant envoi au Driver
    int count = 0;  // Compteur de données reçues

    while (1) {
        // Lecture d'une donnée depuis le pipe venant du Lecteur
        if (read(pipefd[0], &data[count], sizeof(int)) == -1) {
            perror("Erreur de lecture dans le pipe");
            exit(1);
        }

        count++;
        printf("Rédacteur : donnée reçue %d\n", data[count - 1]);

        // Une fois 5 données accumulées, elles sont envoyées au Driver
        if (count == 5) {
            for (int i = 0; i < 5; i++) {
                if (write(pipefd_driver[1], &data[i], sizeof(int)) == -1) {
                    perror("Erreur d'écriture dans le pipe (Rédacteur -> Driver)");
                    exit(1);
                }
            }
            count = 0;  // Réinitialisation du compteur
        }
    }
}
