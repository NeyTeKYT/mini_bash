#define MAX_ARG 100

#include "tp1.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

// Transforme une chaîne de caractères contenant une commande en un tableau d'arguments de type argv
char ** Ligne2ARGV(char * ligne) {
    char ** arg;
    char * li;
    int i;
    char * p;
    li = strdup(ligne); // "strdup" duplique la ligne et la stocke dans "li"
    arg = (char **)malloc(MAX_ARG * sizeof(char *));    // allocation de MAX_ARG pointeurs
    p = strtok(li, " \t\n");
    i = 0;
    while(p) {
        arg[i] = strdup(p);
        i++;
        p = strtok(NULL, "\t\n");
    }
    arg[i] = NULL;
    return arg;
}

// Affiche un tableau de type ARGV
void AfficheArgv(char ** argv) {
    int i = 0;
    while(argv[i] != NULL) {
        printf("%s", argv[i]);
        if(argv[i+1] != NULL) printf(" ");
        i++;
    }
    printf("\n");
}

// Affiche un tableau de type ARGV sans faire d'espace à la fin pour améliorer le rendu de certains programmes
void AfficheArgvWithoutSpace(char ** argv) {
    int i = 0;
    while(argv[i] != NULL) {
        printf("%s", argv[i]);
        if(argv[i+1] != NULL) printf(" ");
        i++;
    }
}

// Transforme un tableau de type ARGV en chaîne de caractères
char * Argv2Ligne(char ** commande) {
    int total_length = 0;   // contient le nombre de caractères de la commande
    int i = 0;
    
    // Calcule la taille de la commande
    while(commande[i] != NULL) {
        total_length += strlen(commande[i]) + 1; // +1 for adding a space
        i++;
    }

    char * c = malloc(total_length);
    if(!c) return NULL;

    i = 0;
    while(commande[i] != NULL) {
        strcat(c, commande[i]); // concatène commande[i] dans c
        strcat(c, "\0"); // adding a space
        i++;
    }

    return c;
}

// Reçoit un argument de type ARGV, l'exécute, attend qu'il se termine et retourne : 
// - sa valeur de retour (EXITSTATUS) ou bien,
// - 255 si le fork n'a pas marché (pb système)
// - 254 si l'exec n'a pas marché (command not found)
int Execute(char ** commande) {
    int pid, res, ret;
    pid = fork();
    if(pid == 0) { // processus fils
        execvp(commande[0], commande); 
        exit(254); // l'exec n'a pas marché (command not found)
    }
    else if(pid > 0) { // père
        res = wait(&ret);
        return(WEXITSTATUS(ret));
    }
    else exit(255); // problème dans le fork (pb système)
}

// Création d'un programme interpréteur de commandes
void MiniBash() {

    char commande[MAX_ARG]; // getchar() returns EOF (commonly -1) when Ctrl + D is pressed.

    printf("Entrer Commande > ");    // affiche un prompt

    char ** argv;

    while(fgets(commande, sizeof(commande), stdin) != NULL) { // tant que l'utilisateur n'a pas appuyé sur Ctrl + D

        // création de argv puis l'exécute
        argv = Ligne2ARGV(commande);
        
        // Traitement des caractères ">", "<", "<<"
        int sauveout = dup(1);
        int sauvein = dup(0);

        for(int i = 0; argv[i] != NULL; i++) {

            if(strcmp(argv[i], ">") == 0) { 
                int file = open(argv[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644); // crée le fichier s'il n'existe pas déjà
                argv[i] = NULL;
                close(1);   // fermeture de STDOUT
                dup(file);  // la sortie se fait dans le fichier 
                close(file);
                break;
            }
            else if(strcmp(argv[i], "<") == 0) {
                int file = open(argv[i+1], O_RDONLY);
                argv[i] = NULL;
                close(0);   // fermeture de STDIN
                dup(file);  // l'entrée se fait via le fichier
                close(file);
                break;
            }
            else if(strcmp(argv[i], ">>") == 0) {
                int file = open(argv[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                argv[i] = NULL;
                close(1);
                dup(file);
                close(file);
                break;
            }

        }

        Execute(argv);

        close(0); dup(sauvein); close(sauvein);
        close(1); dup(sauveout); close(sauveout);

        // libération de la mémoire
        int i = 0;
        while(argv[i] != NULL) {
            free(argv[i]);
            i++;
        }
        free(argv);

        printf("Entrer Commande > ");    // affiche un prompt
        
    }

    printf("\nEOF détecté, fermeture du programme.\n");

}

// Lit des commandes dans un fichier, les transforme en Argv et les range toutes dans un tableau qu'il retourne.
// Il retourne en plus dans le 2ème argument le nombre de commandes lues.
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

    char line[MAX_ARG];
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

// Affiche un TabArgv
void AfficheTabArgv(char *** TabArgv) {
    for(int i = 0; i < sizeof(TabArgv); i++) {
        AfficheArgv(TabArgv[i]);
    }
}

// Prend en argument un nom de fichier contenant des commandes, les stocke dans un tableau (File2TabArgv),
// puis les exécute une à une et affiche "FIN" une fois que toutes les commandes sont exécutées
void ExecFile(char * fichier) {

    int nbCommandesLues = 0;
    char *** TabArgv = File2TabArgv(fichier, &nbCommandesLues);  // tableau contenant toutes les commandes

    for(int i = 0; i < nbCommandesLues; i++) {  // ex : le fichier a 8 lignes, donc il devrait prendre au moins 40s pour exécuter toutes les commandes
        //AfficheArgv(TabArgv[i]);
        int retour = Execute(TabArgv[i]);
        //sleep(5);   // attend 5 secondes avant d'exécuter la prochaine commande
    }

    printf("FIN");  // toutes les commandes sont exécutées

}

// Reçoit un argument de type ARGV, l'exécute, mais n'attend pas qu'il se termine.
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

// Prend en argument un nom de fichier contenant des commandes, les stocke dans un tableau (File2TabArgv),
// les exécute TOUTES EN MÊME TEMPS et affiche FIN une fois que toutes les commandes sont exécutées.
// ./exec_file wait.txt : attend 32 secondes
// ./exec_file_batch wait.text : attend 20 secondes
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

    printf("FIN\n");  // toutes les commandes sont exécutées

}

// on déclarera une variable comme ceci : ENRCOMM TabCom;
// et ENRCOMM * TabCom; pour un tableau.

// Remplira un tableau ENRCOMM à partir des lignes du fichier passé en paramètres
ENRCOMM * File2TabCom(char * fichier, int * nbCommandesLues) {

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

    ENRCOMM * TabCom = malloc(nbLinesInFile * sizeof(ENRCOMM));

    char line[MAX_ARG];
    int i = 0;
    *nbCommandesLues = 0;

    while(fgets(line, sizeof(line), file)) {    // read each line from the file and store it in the 'line' buffer.
        
        ENRCOMM e;

        char ** argv = Ligne2ARGV(line);
        e.argv = argv;
        e.statut = -1;  // on ne demande pas d'exécuter la commande donc -1
        e.pid = e.retour = -1;
        e.debut = e.fin = 0;

        TabCom[i] = e;  // ajouter au tableau une fois correctement initialisé

        i++;
        (*nbCommandesLues)++;
    }

    fclose(file);   // fermeture du fichier : on a terminé de le parcourir

    return TabCom;

}

void AfficheENRCOMM(ENRCOMM e) {
    AfficheArgv(e.argv);
    if(e.pid == -1) {
        printf("Status : %d\n\n", e.statut);
    }
    else {
        printf("PID : %d\n", e.pid);
        printf("Status : %d\n", e.statut);
    }
    if(e.retour != -1) printf("Retour : %d\n", e.retour);
    if(e.debut > 0) printf("Début : %ld\n", e.debut);
    if(e.fin > 0) printf("Fin : %ld\n\n", e.fin);
    //printf("\n\n");
}

void AfficheTabENRCOMM(ENRCOMM * TabCom, int nbCommandesLues) {
    for(int i = 0; i < nbCommandesLues; i++) {
        AfficheENRCOMM(TabCom[i]);
    }
}

ENRCOMM ExecuteENRCOMM(char ** commande, ENRCOMM e) {
    int pid, res, ret;
    time_t debut_execution_commande; time(&debut_execution_commande); e.debut = debut_execution_commande;
    pid = fork();

    // fils
    if(pid == 0) {
        e.pid = 0;
        execvp(commande[0], commande);    // si ce code réussit, "exit(254)" n'est jamais exécuté, car le processue est remplacé
        exit(254);  // est exécuté si execvp() échoue pour signaler une erreur d'exécution
    }

    // père
    else if(pid > 0) {
        e.pid = pid;    // le PID du fils est retourné au père
        e.statut = 1;   // en cours d'exécution
        res = wait(&ret);
        e.retour = WEXITSTATUS(ret);
    }

    else {
        e.pid = -1;
        exit(255);
    } 

    return e;
}

// Affiche un rapport d'exécution de toutes les commandes exécutées, ainsi que le temps total.
void ExecFileENRCOMM(char * fichier) {
    int nbCommandesLues = 0;
    ENRCOMM * TabCom = File2TabCom(fichier, &nbCommandesLues);  // tableau contenant toutes les commandes 

    time_t debut_execution; time(&debut_execution);
    for(int i = 0; i < nbCommandesLues; i++) {  
        ENRCOMM e;

        e.argv = TabCom[i].argv;
        e = ExecuteENRCOMM(e.argv, e); // toutes les autres valeurs (sauf fin) de e sont initialisées directement dans la fonction car on fournit l'objet e
        time_t fin_execution_commande; time(&fin_execution_commande);
        e.fin = fin_execution_commande;

        TabCom[i] = e;
    }
    time_t fin_execution; time(&fin_execution);

    printf("FIN\n\n");  // toutes les commandes sont exécutées

    // le temps total d'exécution de toutes les commandes du fichier
    time_t duree_execution = fin_execution - debut_execution;   
    printf("Temps d'exécution de toutes les commandes du fichier : %ld secondes.\n", duree_execution);

    AfficheTabENRCOMM(TabCom, nbCommandesLues);  // affiche un rapport d'exécution de toutes les commandes exécutées
}

int ExecuteENRCOMMBatch(char ** commande, ENRCOMM * e) {
    int pid, res, ret;
    time_t debut_execution_commande; time(&debut_execution_commande); e->debut = debut_execution_commande;
    pid = fork();

    // fils
    if(pid == 0) { 
        e->pid = 0;
        if(execvp(commande[0], commande)) return WEXITSTATUS(res); 
        else exit(254); // l'exec n'a pas marché (command not found)
    }

    // père
    else if(pid > 0) { 
        time_t fin_execution_commande; time(&fin_execution_commande); e->fin = fin_execution_commande;
        e->pid = pid;    // le PID du fils est retourné au père
        e->statut = 1;   // en cours d'exécution
        e->retour = WEXITSTATUS(ret);
        return WEXITSTATUS(ret);
    }

    else {
        e->pid = -1;
        exit(255); // problème dans le fork (pb système)
    }

} 

// Affiche un rapport d'exécution DÈS QU'UNE COMMANDE VIENT DE SE TERMINER
// Puis à la fin de toutes les commandes, affiche "FIN" et un rapport complet sur toutes les commandes, ainsi que le temps total
void ExecFileBatchENRCOMM(char * fichier) {
    int nbCommandesLues = 0;
    ENRCOMM * TabCom = File2TabCom(fichier, &nbCommandesLues);  // tableau contenant toutes les commandes

    time_t debut_execution; time(&debut_execution);
    for(int i = 0; i < nbCommandesLues; i++) { 
        ENRCOMM e;

        e.argv = TabCom[i].argv;
        int code_retour = ExecuteENRCOMMBatch(e.argv, &e);  // toutes les autres valeurs de e sont initialisés directement dans la fonction car on fournit l'objet e

        TabCom[i] = e;
    }

    for(int i = 0; i < nbCommandesLues; i++) {
        int pid = wait(NULL);
        for(int j = 0; j < nbCommandesLues; j++) {
            if(TabCom[j].pid == pid) {
                time_t fin_execution_commande; time(&fin_execution_commande);
                TabCom[j].fin = fin_execution_commande;
                AfficheENRCOMM(TabCom[j]);
            }
        }
    }
    time_t fin_execution; time(&fin_execution);

    printf("FIN\n\n");  // toutes les commandes sont exécutées

    // le temps total d'exécution de toutes les commandes du fichier
    time_t duree_execution = fin_execution - debut_execution;   
    printf("Temps d'exécution de toutes les commandes du fichier : %ld secondes.", duree_execution);
    printf("\n\n");

    AfficheTabENRCOMM(TabCom, nbCommandesLues);  // affiche un rapport d'exécution de toutes les commandes exécutées
}

// Une version amméliorée de ExecFileBatchLimite mais qui peut exécuter N processus (commandes) en même temps.
// -> Pour éviter qu'un nombre trop important de commandes soient exécutés par la machine en même temps.
void ExecFileBatchLimite(char * fichier, int N) {
    int nbCommandesLues = 0;
    ENRCOMM * TabCom = File2TabCom(fichier, &nbCommandesLues);  // tableau contenant toutes les commandes

    printf("Nombre de processus maximum à exécuter en même temps : %d\n\n", N);
    int nb_executing_process = 0;

    time_t debut_execution; time(&debut_execution);
    for(int i = 0; i < nbCommandesLues; i++) { 
        ENRCOMM e;

        e.argv = TabCom[i].argv;
        if(nb_executing_process == N) {
            int pid = wait(NULL);
            for(int j = 0; j < nbCommandesLues; j++) {
                if(TabCom[j].pid == pid) {
                    time_t fin_execution_commande; time(&fin_execution_commande);
                    TabCom[j].fin = fin_execution_commande;
                    AfficheENRCOMM(TabCom[j]);
                }
            }
            nb_executing_process--;
        }
        int code_retour = ExecuteENRCOMMBatch(e.argv, &e);  // toutes les autres valeurs de e sont initialisés directement dans la fonction car on fournit l'objet e
        nb_executing_process++;
        printf("- Lancement de "); AfficheArgvWithoutSpace(e.argv); printf(" à %ld.\n\n", e.debut); 

        TabCom[i] = e;
    }

    for(int i = 0; i < nbCommandesLues; i++) {
        int pid = wait(NULL);
        for(int j = 0; j < nbCommandesLues; j++) {
            if(TabCom[j].pid == pid) {
                time_t fin_execution_commande; time(&fin_execution_commande);
                TabCom[j].fin = fin_execution_commande;
                AfficheENRCOMM(TabCom[j]);
            }
        }
    }
    time_t fin_execution; time(&fin_execution);

    printf("\n\nFIN\n\n");  // toutes les commandes sont exécutées

    // le temps total d'exécution de toutes les commandes du fichier
    time_t duree_execution = fin_execution - debut_execution;   
    printf("Temps d'exécution de toutes les commandes du fichier : %ld secondes.", duree_execution);
    printf("\n\n");

    AfficheTabENRCOMM(TabCom, nbCommandesLues);  // affiche un rapport d'exécution de toutes les commandes exécutées
}