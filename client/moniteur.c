/* moniteur.c */

#include "CL_include"
#include <sys/shm.h>

int msqid;  // Identifiant de la messagerie pour la communication avec le serveur
dmsgbuf message;  // Structure pour envoyer et recevoir des messages
BUF *tampons;  // Pointeur vers la mémoire partagée pour les tampons de données

// Fonction de connexion au serveur
void connect_to_server() {
    int pid = getpid();  // PID du client

    // Préparation et envoi d'une demande de connexion au serveur
    message.type = CONNECT;
    sprintf(message.txt, "%d", pid);

    if (msgsnd(msqid, &message, strlen(message.txt) + 1, 0) == -1) {
        perror("Erreur d'envoi du message de connexion");
        exit(1);
    }

    // Réception de la réponse du serveur contenant la clé d'accès à la mémoire partagée
    if (msgrcv(msqid, &message, L_MSG, pid, 0) == -1) {
        perror("Erreur de réception du message");
        exit(1);
    }

    // Vérification de la clé reçue
    if (strlen(message.txt) == 0) {
        printf("Serveur saturé, impossible de se connecter.\n");
        exit(1);
    } else {
        printf("Connexion réussie, clé d'accès reçue : %s\n", message.txt);
    }

    // Envoi d'un message d'acquittement pour confirmer la connexion
    message.type = ACK;
    sprintf(message.txt, "%d", pid);
    if (msgsnd(msqid, &message, strlen(message.txt) + 1, 0) == -1) {
        perror("Erreur d'envoi du message ACK");
        exit(1);
    }
}

// Fonction pour configurer la messagerie avec le serveur
void setup_messaging() {
    key_t key = ftok(CleServeur, C_Msg);
    msqid = msgget(key, 0666);  // Création de la file de messages
    if (msqid == -1) {
        perror("Erreur d'initialisation de la messagerie");
        exit(1);
    }
}

// Fonction pour initialiser la mémoire partagée pour stocker les tampons
void init_shared_memory() {
    key_t shm_key = ftok(CleClient, C_Shm);  // Génération de la clé pour la mémoire partagée
    int shmid = shmget(shm_key, sizeof(BUF) * NVOIES, IPC_CREAT | 0666);  // Allocation de mémoire partagée
    if (shmid == -1) {
        perror("Erreur d'initialisation de la mémoire partagée");
        exit(1);
    }

    // Attachement de la mémoire partagée
    tampons = (BUF *)shmat(shmid, NULL, 0);
    if (tampons == (void *)-1) {
        perror("Erreur d'attachement de la mémoire partagée");
        exit(1);
    }

    // Initialisation des tampons (chaque voie commence avec un index à -1)
    for (int i = 0; i < NVOIES; i++) {
        tampons[i].n = -1;
    }

    
}


// Fonction pour se deconnecter du serveur
void disconnect_from_server() {
    int pid = getpid();  // PID du client

    // Préparation et envoi d'un message de déconnexion
    message.type = DECONNECT;
    sprintf(message.txt, "%d", pid);

    if (msgsnd(msqid, &message, strlen(message.txt) + 1, 0) == -1) {
        perror("Erreur d'envoi du message de déconnexion");
    } else {
        printf("Client : Déconnexion envoyée au serveur.\n");
    }
}

