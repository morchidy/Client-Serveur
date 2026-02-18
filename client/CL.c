/* CL.c */

#include <signal.h>
#include "CL_include"
#include "moniteur.h"
#include "lecteur.h"
#include "redacteur.h"
#include "driver.h"


int voie;  // Variable globale pour savoir quelle voie est lue
int pipefd[2];  // Descripteur du pipe entre Lecteur et Rédacteur
int pipefd_driver[2];  // Pipe entre Rédacteur et Driver
//BUF *tampons;  // Tampons partagés (sera initialisé par le moniteur)

// Gestionnaire de SIGINT (Ctrl+C)
void handle_sigint(int sig) {
    printf("\nClient : Interruption reçue (Ctrl+C).\n");
    disconnect_from_server();  // Envoie une demande de déconnexion
    signal(SIGINT, SIG_DFL);// Désactiver le gestionnaire de SIGINT
    exit(0);  // Termine proprement le client
}


int main(int argc, char *argv[]) {
    int nb_iteration = 25;  // Durée par défaut du client

    if (argc > 1) {
        nb_iteration = atoi(argv[1]);
    }

    signal(SIGINT, handle_sigint);  // Capture SIGINT pour gérer Ctrl+C

    printf("\n* * * * * * * * * * * * * * * * * * * * * * * * *\n");
    printf("Projet CLIENT - SERVEUR\tTemps Réel E2i - Novembre\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * * * *\n");
    printf("\nLancement du client pendant %d secondes\n", nb_iteration);

    // Initialisation du pipe
    if (pipe(pipefd) == -1 || pipe(pipefd_driver) == -1) {
        perror("Erreur de création du pipe");
        exit(1);
    }

    // Initialisation de la messagerie et connexion au serveur
    setup_messaging();  // Fonction dans moniteur.c
	init_shared_memory(); //initialiser la mémoire partagée
    connect_to_server();  // Fonction dans moniteur.c

    // Création du processus Rédacteur
    if (fork() == 0) {
        // Processus enfant : Rédacteur
        recevoir_donnees();  // Fonction dans redacteur.c
        exit(0);
    }

    // Création du processus Driver
    if (fork() == 0) {
        // Processus enfant : Driver
        afficher_donnees();  // Fonction dans driver.c
        exit(0);
    }

    // Mise en place des gestionnaires de signaux pour le Lecteur
    signal(SIGUSR1, litbuf1);
    signal(SIGUSR2, litbuf2);

    // Boucle principale pour la durée spécifiée
    int start_time = time(NULL);
    while (time(NULL) - start_time < nb_iteration) {
        pause();  // Attente des signaux (SIGUSR1, SIGUSR2)
    }

    // Terminaison du programme client après la durée définie
    disconnect_from_server();
    printf("Fin du client après %d secondes.\n", nb_iteration);
    return 0;
}



