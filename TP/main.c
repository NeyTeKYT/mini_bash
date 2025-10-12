#include "fonctions.h"
#include <stdio.h>

void exercice1() {
    char ** commande = Ligne2ARGV("ls -l");
}

void exercice2() {
    char ** commande = Ligne2ARGV("ls -l");
    AfficheArgv(commande);
}

void exercice3() {
    char ** commande = Ligne2ARGV("ls -l");
    char * ligne = Argv2Ligne(commande);
}

void exercice4() {
    char ** commande = Ligne2ARGV("sleep 10");
    int retour = Execute(commande);
}

void exercice6() {
    int nbCommandesLues;
    char *** TabArgv = File2TabArgv("commandes.txt", &nbCommandesLues);
    printf("%d\n", nbCommandesLues);
}

void exercice8() {
    char ** commande = Ligne2ARGV("sleep 10");
    int retour = ExecuteBatch(commande);
}

void exercice10(char * fichier) {
    ENRCOMM * TabCom;
    int nbCommandesLues;
    TabCom = File2TabCom(fichier, &nbCommandesLues);
    printf("%d\n", nbCommandesLues);
    AfficheTabENRCOMM(TabCom);
}

void exercice11(char * fichier) {
    ExecFileENRCOMM(fichier);
}

int main(int argc, char * argv[]) {

    //exercice1();

    //exercice2();

    //exercice3();

    //exercice4();    // attend 10 secondes

    //MiniBash(); // exercie 5

    //exercice6();

    //ExecFile("commandes.txt");  // exercice 7

    //exercice8();    // n'attend pas 10 secondes

    //ExecFileBatch("wait.txt");

    //exercice10(argv[1]);

    exercice11(argv[1]);
    
    return 0;

}