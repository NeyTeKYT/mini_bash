#ifndef TP3
#define TP3

#include <stdbool.h>

// Exercice 1
void Init();
void ReserveWithoutLockf(int n);
void ReserveWithLockf(int n);

int Ls(char * repertoire);  // Exercice 2
int LsDansFichier(char * repertoire, char * fichierResultat);   // Exercice 3
int nbFichiers(char * repertoire);  // Exercice 4/5

#endif