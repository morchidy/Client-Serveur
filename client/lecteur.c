/* lecteur.c */

#include "CL_include"
#include <unistd.h>

extern int voie;  // Numéro de la voie active (0 pour voie 1, 1 pour voie 2)
extern BUF *tampons;  // Pointeur vers les tampons partagés
extern int pipefd[2];  // Pipe pour envoyer les données au rédacteur

// Fonction pour lire les données du tampon correspondant à la voie active
void lire_tampon() {
    int n = tampons[voie].n;  // Récupération de l'index de la dernière donnée dans le tampon
    int data = tampons[voie].tampon[n];  // Récupération de la donnée dans le tampon

    // Envoi de la donnée lue au rédacteur via le pipe
    if (write(pipefd[1], &data, sizeof(int)) == -1) {
        perror("Erreur d'écriture dans le pipe");
        exit(1);
    }

    printf("Donnée envoyée au rédacteur : %d\n", data);
}

// Gestionnaire de signal pour la voie 1
void litbuf1() {
    voie = 0;  // On lit dans la voie 1
    lire_tampon();
    signal(SIGUSR1, litbuf1);  // Réinstallation du gestionnaire de signal
}

// Gestionnaire de signal pour la voie 2
void litbuf2() {
    voie = 1;  // On lit dans la voie 2
    lire_tampon();
    signal(SIGUSR2, litbuf2);  // Réinstallation du gestionnaire de signal
}
