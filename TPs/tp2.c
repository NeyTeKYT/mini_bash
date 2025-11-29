#define MAX_ARG 100

#include "tp1.h"
#include "tp2.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// Affiche le message "message" chaque seconde, pendant N secondes.
void Affiche(char ** message, int N) {
    int nb_commandes_executees = 0;
    while(nb_commandes_executees < N) {
        AfficheArgvWithoutSpace(message); printf(" (%d)", nb_commandes_executees+1); printf("\n");
        sleep(1);   // Commande qui attend 1 seconde
        nb_commandes_executees++;
    }
}

// Vérifie le mot de passe entré par l'utilisateur
void verifierPassword(int sig) {
    char buffer[MAX_ARG];

    printf("Entrez un mot de passe : ");
    fgets(buffer, sizeof(buffer), stdin);

    if(strcmp(buffer, "password\n") == 0) {  // Le mot de passe est "password"
        printf("Vous avez bien entré le bon mot de passe !\n");
        //kill(getpid(), SIGKILL);
        exit(1);    // "exit(1)" indique une terminaison annormale du programme
    }
    else {
        printf("Vous n'avez pas entré le bon mot de passe !\n");
    }
}

// Affiche le message "message" chaque seconde, pendant N secondes.
// Capte le <CTRL C> et se tue seulement si l'utilisateur tape un mot de passe correct.
void Affiche2(char ** message, int N) {
    signal(SIGINT, verifierPassword);
    int nb_commandes_executees = 0;
    while(nb_commandes_executees < N) {
        AfficheArgvWithoutSpace(message); printf(" (%d)", nb_commandes_executees+1); printf("\n");
        sleep(1);   // Commande qui attend 1 seconde
        nb_commandes_executees++;
    }
}

// Tue le processus / la commande en cours d'exécution
void timeOutAlarm(int sig) {
    printf("Processus tué car il n'est pas terminé !\n");
    exit(1);
}

// Lance un processus / une commande, l'arrête si elle n'est pas terminée au bout de N secondes.
void timeOut(char ** commande, int N) {
    ExecuteBatch(commande);
    signal(SIGALRM, timeOutAlarm);
    alarm(N);
    wait(NULL);
}

// Vérifie si toutes les commandes sont terminées ou si il y en a encore en cours d'exécution
bool continueToWaitForCommandsToFinish(ENRCOMM * TabCom, int nbCommandesLues) {
    for(int i = 0; i < nbCommandesLues; i++) {
        if(TabCom[i].statut == 1) return true;
    }
    return false;
}

// Prend en argument un nom de fichier contenant des commandes, les stocke dans un tableau (File2TabArgv),
// les exécute TOUTES EN MÊME TEMPS et affiche FIN une fois que toutes les commandes sont exécutées.
// ./exec_file wait.txt : attend 32 secondes
// ./exec_file_batch wait.text : attend 20 secondes
// Affiche un rapport d'exécution toutes les secondes
void ExecFileBatchReportENRCOMM(char * fichier) {
    int nbCommandesLues = 0;
    ENRCOMM * TabCom = File2TabCom(fichier, &nbCommandesLues);  // tableau contenant toutes les commandes

    time_t debut_execution; time(&debut_execution);
    for(int i = 0; i < nbCommandesLues; i++) { 
        ENRCOMM e;

        e.argv = TabCom[i].argv;
        int code_retour = ExecuteENRCOMMBatch(e.argv, &e);  // toutes les autres valeurs de e sont initialisés directement dans la fonction car on fournit l'objet e

        TabCom[i] = e;
    }

    while(continueToWaitForCommandsToFinish(TabCom, nbCommandesLues)) {

        for(int i = 0; i < nbCommandesLues; i++) {
            if(TabCom[i].statut == 1) { // encore en cours d'exécution
                int status;
                pid_t res = waitpid(TabCom[i].pid, &status, WNOHANG); // WNOHANG returns immediately if no child has exited.

                if(res == TabCom[i].pid) {  // vient de se terminer
                    TabCom[i].statut = 0;   // 0 = terminé
                    time_t fin_execution_commande; time(&fin_execution_commande); TabCom[i].fin = fin_execution_commande;
                }
            }
        }

        AfficheTabENRCOMM(TabCom, nbCommandesLues);
        sleep(1);
        
    }

    time_t fin_execution; time(&fin_execution);

    printf("FIN\n\n");  // toutes les commandes sont exécutées

    // le temps total d'exécution de toutes les commandes du fichier
    time_t duree_execution = fin_execution - debut_execution;   
    printf("Temps d'exécution de toutes les commandes du fichier : %ld secondes.", duree_execution);
    printf("\n\n");

    AfficheTabENRCOMM(TabCom, nbCommandesLues);  // affiche un rapport d'exécution de toutes les commandes exécutées
}