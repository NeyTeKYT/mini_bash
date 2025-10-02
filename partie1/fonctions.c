#define MAX_ARG 100
#include "fonctions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// exercice 1
char **Ligne2ARGV(char*ligne) {
    char **arg;
    char *li;
    int i;
    char *p;
    li = strdup(ligne); // strtok peut modifier la ligne, on le met dans li, strdup alloue et copie
    arg = (char **)malloc(MAX_ARG * sizeof(char *));
    p = strtok(li," \t\n");
    i = 0;
    while(p) {
        arg[i] = strdup(p);
        i++;
        p = strtok(NULL, "\t\n");
    }
    arg[i] = NULL;
    return arg;
}

// exercice 2
void AfficheArgv(char ** array) {
    printf("\n");
    for(int i = 0; i < sizeof(array); i++) {
        //printf("Arg ", i, " : ", array[i], "\n"); 
        printf(array[i], "\n"); // il manque un espace entre chaque tour de boucle
    }
    printf("\n");
}

// exercice 3
char *Argv2Ligne(char ** commande) {
    int total_length = 0;
    int i = 0;
    
    // calculate total length
    while(commande[i] != NULL) {
        total_length += strlen(commande[i]) + 1; // +1 for adding a space
        i++;
    }

    char * c = malloc(total_length);
    if(!c) return NULL;

    i = 0;
    while(commande[i] != NULL) {
        strcat(c, commande[i]);
        strcat(c, "\0"); // adding a space
        i++;
    }

    return c;
}

// exercice 4
int Execute(char ** commande) {
    int pid, res, ret;
    pid = fork();
    if(pid == 0) { // processus fils
        if(execvp(commande[0], commande)) return WEXITSTATUS(res); 
        else exit(254); // l'exec n'a pas marché (command not found)
    }
    else if(pid > 0) { // père
        res = wait(&ret);
        return(WEXITSTATUS(ret));
    }
    else exit(255); // problème dans le fork (pb système)
}

// exercice 5
void MiniBash() {

    char commande[100]; // getchar() returns EOF (commonly -1) when Ctrl + D is pressed.

    printf("Entrer Commande >");    // affiche un prompt

    while(fgets(commande, sizeof(commande), stdin) != NULL) { // tant que l'utilisateur n'a pas appuyé sur Ctrl + D

        Execute(Ligne2ARGV(commande));  // création de argv puis l'exécute
        printf("Entrer Commande >");    // affiche un prompt
        
    }

    printf("\nEOF détecté, fermeture du programme.\n");

}

// exercice 6
char *** File2TabArgv(char * fichier, int * nbCommandesLues) {

    FILE * file = fopen(fichier, "r");  // ouverture du fichier en lecture seule (r)

    if(file == NULL) return NULL;   // check if the file was opened successfully.
    
    int nbLinesInFile = 0;
    char c;

    for(c = getc(file); c != EOF; c = getc(file)) {
        if(c == '\n') {
            nbLinesInFile++;
        }
    }

    rewind(file); // revenir au début du fichier

    char *** TabArgv = malloc(nbLinesInFile * sizeof(char **));

    char line[100];
    int i = 0;
    *nbCommandesLues = 0;

    while(fgets(line, sizeof(line), file)) {    // read each line from the file and store it in the 'line' buffer.
        TabArgv[i] = Ligne2ARGV(line);
        i++;
        (*nbCommandesLues)++;
    }

    fclose(file);   // fermeture du fichier : on a terminé de le parcourir

    return TabArgv;

}

// exercice 7
void ExecFile(char * fichier) {

    int nbCommandesLues = 0;
    char *** TabArgv = File2TabArgv(fichier, &nbCommandesLues);  // tableau contenant toutes les commandes

    for(int i = 0; i < nbCommandesLues; i++) {  // ex : le fichier a 8 lignes, donc il devrait prendre au moins 40s pour exécuter toutes les commandes
        AfficheArgv(TabArgv[i]);
        int resultCommand = Execute(TabArgv[i]);
        sleep(5);   // attend 5 secondes avant d'exécuter la prochaine commande
    }

    printf("FIN");  // toutes les commandes sont exécutées

}

// exercice 8
int ExecuteBatch(char ** commande) {

    int pid, res, ret;
    pid = fork();
    if(pid == 0) { // processus fils
        if(execvp(commande[0], commande)) return WEXITSTATUS(res); 
        else exit(254); // l'exec n'a pas marché (command not found)
    }
    else if(pid > 0) { // père
        return(WEXITSTATUS(ret));
    }
    else exit(255); // problème dans le fork (pb système)

} 

// exercice 9
// avec le fichier "wait.txt", le temps d'exécution doit être de 20 secondes (sleep 20 est le max) 
// != ExecFile("wait.txt") où ça serait sleep 1 + 1 + 10 + 20 = 32 secondes
void ExecFileBatch(char * fichier) {

    int nbCommandesLues = 0;
    char *** TabArgv = File2TabArgv(fichier, &nbCommandesLues);  // tableau contenant toutes les commandes

    for(int i = 0; i < nbCommandesLues; i++) { 
        //AfficheArgv(TabArgv[i]);
        int resultCommand = ExecuteBatch(TabArgv[i]);
    }

    for(int i = 0; i < nbCommandesLues; i++) {
        wait(NULL); // attend qu'un fils (crée avec ExecuteBatch, n'importe lequel) se termine, dans l'ordre ça donne sleep 1, sleep 1, sleep 10, sleep 20
    }

    printf("\nFIN\n");  // toutes les commandes sont exécutées

}

