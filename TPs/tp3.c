#define MAX_ARG 100

#include "tp1.h"

#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

/**
    Initialise le fichier binaire base.dat contenant 4 entiers ayant pour valeur 100.
*/
void Init() {
    int file = open("base.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    int value = 100;    // Valeur attribuée aux 4 entiers
    for(int i = 0; i < 4; i++) write(file, &value, sizeof(int));

    close(file);    // Bien penser à fermer le fichier après l'avoir utilisé
}

/**
    Décrémente l'entier n du fichier binaire base.dat s'il n'est pas déjà égal à 0.
*/
void ReserveWithoutLockf(int n) {
    int file = open("base.dat", O_RDWR);    // O_RDWR = lecture / écriture

    int value;  // Buffer dans lequel on va stocker la valeur de l'entier qu'on décrémentera
    off_t offset = n * sizeof(int); // décrit où se placer dans le fichier
    lseek(file, offset, SEEK_SET);  
    read(file, &value, sizeof(int));

    // Ces deux lignes permettent de tester l'exécution du programme en utilisant 2 terminaux
    printf("%d\n", value);  // affiche la valeur avant d'être décrémentée
    getchar();  // permet d'attendre que l'utilisateur entre un caractère

    if(value > 0) { // SI la valeur n'est pas nulle = peut encore être décrémentée 
        value--;    // Décrémente la valeur
        lseek(file, offset, SEEK_SET);  // retourne à l'emplacement de la valeur
        write(file, &value, sizeof(int));   // écrit la nouvelle valeur décrémentée
    }

    close(file);
}

/**
    Décrémente l'entier n du fichier binaire base.dat s'il n'est pas déjà égal à 0.
    Utilise lockf pour permettre de ne pas cibler la même valeur quand on exécute le programme en même temps dans 2 terminaux.
*/
void ReserveWithLockf(int n) {
    int file = open("./base.dat", O_RDWR);

    int value;  // Buffer dans lequel on va stocker la valeur de l'entier qu'on décrémentera
    off_t offset = n * sizeof(int);
    lseek(file, offset, SEEK_SET);

    lockf(file, F_LOCK, sizeof(int));   // lock le fichier une fois bien placé

    read(file, &value, sizeof(int));

    // Ces deux lignes permettent de tester l'exécution du programme avec 2 terminaux
    printf("%d\n", value);  // affiche la valeur avant d'être décrémentée
    getchar();  // permet d'attendre que l'utilisateur entre un caractère

    if(value > 0) {
        value--;
        lseek(file, offset, SEEK_SET);
        write(file, &value, sizeof(int));   
    }

    lockf(file, F_ULOCK, sizeof(int));  // unlock une fois la valeur décrémentée

    close(file);
}

/**
    Affiche la liste des fichiers contenus dans "repertoire".
*/
int Ls(char * repertoire) {
    int pid, res, ret;
    pid = fork();

    if(pid == 0) {  // processus fils

        char ligne[MAX_ARG];
        strcpy(ligne, "ls ");   // copie "ls " dans ligne AVEC UN ESPACE POUR NE PAS FAIRE "ls./" !!
        strcat(ligne, repertoire);  // concatène la ligne avec le répertoire
        char ** argv = Ligne2ARGV(ligne);

        execvp("ls", argv);
        exit(254); // l'exec n'a pas marché (command not found)
    }
    else if(pid > 0) {  // processus père
        res = wait(&ret);
        return(WEXITSTATUS(ret));
    }
    else exit(255); // l'exec n'a pas marché (command not found)
}

/**
    Écrit la liste des fichiers contenus dans "repertoire" dans le fichier "fichierResultat".
*/
int LsDansFichier(char * repertoire, char * fichierResultat) {
    int pid, res, ret;
    pid = fork();

    if(pid == 0) {  // processus fils
        int file = open(fichierResultat, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        close(1);   // ferme STDOUT
        dup(file);  // file est la nouvelle sortie STDOUT

        Ls(repertoire);
    }
    else if(pid > 0) {  // processus père
        res = wait(&ret);
        return(WEXITSTATUS(ret));
    }
    else exit(255); // l'exec n'a pas marché (command not found)
}

/**
    Affiche le nombre de fichiers contenus dans "repertoire".
*/
int nbFichiers(char * repertoire) {

    int pid1, pid2, resw, retw, t[2];
    pipe(t);
    pid1 = fork();

    if(pid1 == 0) { // exécution de ls
        // redirection de stdout vers t
        close(1);   // ferme STDOUT
        dup(t[1]);  // t[1] est la nouvelle sortie STDOUT
        // fermeture de T
        close(t[1]);
        close(t[0]);  

        //execlp("ls", "ls", repertoire, NULL);
        // utilise la fonction déjà implémentée
        Ls(repertoire);

        exit(254);  // l'exec n'a pas marché (command not found)
    }
    else if(pid1 > 0) {
        pid2 = fork();

        if(pid2 == 0) { // exécution du wc -l
            close(0); // ferme STDIN
            dup(t[0]); // t[0] est la nouvelle entrée STDIN
            // fermeture de T
            close(t[1]); 
            close(t[0]); 

            execlp("wc", "wc", "-l", NULL);

            exit(254);  // l'exec n'a pas marché (command not found)
        }
        else if(pid2 > 0) {
            close(t[1]); close(t[0]);   // fermeture de t
            resw = wait(&retw); resw = wait(&retw); // attendre la fin des 2 fils
            return(WEXITSTATUS(retw));
        }
        else {  // pid 2 < 0 : ls a été lancé, mai pas wc, il faut tuer ls et wait
            close(t[1]); close(t[0]);   // fermeture de t
            kill(pid1, SIGKILL);    // kill ls
            resw = wait(&retw);
            return 252; 
        }
    }
    else {
        close(t[1]); close(t[0]);
        return 255;
    }

}