#ifndef TP1
#define TP1

#include <time.h>

// Partie 1
char ** Ligne2ARGV(char * ligne);   // exercice 1

// exercice 2
void AfficheArgv(char ** argv);    
void AfficheArgvWithoutSpace(char ** argv);

char * Argv2Ligne(char ** commande);    // exercice 3
int Execute(char ** commande);  // exercice 4
void MiniBash();    // exercice 5
char *** File2TabArgv(char * fichier, int * nbCommandesLues);   // exercice 6
void AfficheTabArgv(char *** TabArgv);
void ExecFile(char * fichier);  // exercice 7
int ExecuteBatch(char ** commande); // exercice 8
void ExecFileBatch(char * fichier); // exercice 9

// Partie 2

// Structure pour chaque commande
typedef struct ENRCOMM {
    int pid;    // numéro de processus dans lequel s'exécute la commande
    int statut; // -1 = pas encore exécuté / 0 = terminé / 1 = en exécution
    int retour; // EXITSTATUS (comme dans Execute)
    time_t debut;   // l'epoch à laquelle la commande a été lancée
    time_t fin; // l'epoch à laquelle la commande s'est terminée
    char ** argv;   // Tableau Argv de la commande
} ENRCOMM;

ENRCOMM * File2TabCom(char * fichier, int * nbCommandesLues);  // exercice 10
void AfficheENRCOMM(ENRCOMM e);
void AfficheTabENRCOMM(ENRCOMM * TabCom, int nbCommandesLues);
ENRCOMM ExecuteENRCOMM(char ** commande, ENRCOMM e);
void ExecFileENRCOMM(char * fichier);  // exercice 11
int ExecuteENRCOMMBatch(char ** commande, ENRCOMM * e);
void ExecFileBatchENRCOMM(char * fichier); // exercice 12
void ExecFileBatchLimite(char * fichier, int N); // exercice 13

#endif