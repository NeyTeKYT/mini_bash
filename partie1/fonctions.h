#ifndef FONCTIONS
#define FONCTIONS

char ** Ligne2ARGV(char * ligne);   // exercice 1
void AfficheArgv(char ** array);    // exercice 2
char * Argv2Ligne(char ** commande);    // exercice 3
int Execute(char ** commande);  // exercice 4
void MiniBash();    // exercice 5
char *** File2TabArgv(char * fichier, int * nbCommandesLues);    // exercice 6
void ExecFile(char * fichier);  // exercice 7
int ExecuteBatch(char ** commande); // exercice 8
void ExecFileBatch(char * fichier); // exercice 9

#endif