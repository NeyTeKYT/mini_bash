#ifndef TP2
#define TP2

#include "tp1.h"

#include <stdbool.h>
#include <time.h>

void Affiche(char ** message, int N);   // exercice 1

// exercice 2
void verifierPassword(int sig);
void Affiche2(char ** message, int N);  

// exercice 3
void timeOutAlarm(int sig);
void timeOut(char ** commande, int N);

// exercice 4
bool continueToWaitForCommandsToFinish(ENRCOMM * TabCom, int nbCommandesLues);
void ExecFileBatchReportENRCOMM(char * fichier); 

#endif