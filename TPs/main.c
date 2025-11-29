#include "tp1.h"
#include "tp2.h"
#include "tp3.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {

    // =============================================================================================================

    // TP1 :

    // ./Ligne2ARGV "ls -l"
    /*char ** commande = Ligne2ARGV(argv[1]);
    AfficheArgv(commande);*/

    // ./Argv2Ligne "ls -l"
    /*char ** commande = Ligne2ARGV(argv[1]);
    char * ligne = Argv2Ligne(commande);
    printf("%s\n", ligne);*/

    // ./Execute "ls -l"
    /*char ** commande = Ligne2ARGV(argv[1]);
    int retour = Execute(commande);
    printf("%d\n", retour);*/

    // ./MiniBash
    //MiniBash();

    // ./File2TabArgv commandes.txt
    /*char *** TabArgv;
    int nbCommandesLues;
    TabArgv = File2TabArgv(argv[1], &nbCommandesLues);
    AfficheTabArgv(TabArgv);
    printf("%d\n", nbCommandesLues);*/

    // ./ExecFile commandes.txt
    //ExecFile(argv[1]);

    // ./ExecuteBatch "sleep 10"
    // n'attendra pas les 10 secondes
    /*char ** commande = Ligne2ARGV(argv[1]);
    ExecuteBatch(commande);*/

    // ./ExecFileBatch wait.txt
    //ExecFileBatch(argv[1]);

    // ./File2TabCom commandes.txt
    /*ENRCOMM * TabCom;
    int nbCommandesLues;
    TabCom = File2TabCom(argv[1], &nbCommandesLues);
    AfficheTabENRCOMM(TabCom, nbCommandesLues);*/

    // ./ExecFileENRCOMM commandes.txt
    //ExecFileENRCOMM(argv[1]);

    // ./ExecFileBatchENRCOMM commandes.txt
    //ExecFileBatchENRCOMM(argv[1]);

    // ./ExecFileBatchLimite commandes.txt
    //ExecFileBatchLimite(argv[1], atoi(argv[2]));

    // =============================================================================================================

    // TP2 :

    // ./Affiche "Bonjour" 5
    //Affiche(Ligne2ARGV(argv[1]), atoi(argv[2]));

    // ./AffichePassword "Bonjour" 50
    //Affiche2(Ligne2ARGV(argv[1]), atoi(argv[2]));

    // ./TimeOut "sleep 20" 10
    // ./TimeOut "ls" 10
    //timeOut(Ligne2ARGV(argv[1]), atoi(argv[2]));

    // ./ExecFileBatchReport commandes.txt
    // ./ExecFileBatchReport wait.txt
    //ExecFileBatchReportENRCOMM(argv[1]);

    // =============================================================================================================

    // TP3 :

    // ./Init
    //Init();

    // ./ReserveWithoutLockf 2
    //ReserveWithoutLockf(atoi(argv[1]));

    // ./ReserveWithLockf 2
    //ReserveWithLockf(atoi(argv[1]));

    // ./ls ./
    // ./ls ../
    //Ls(argv[1]);

    // ./ls_dans_fichier ./ fichierResultat.txt
    //LsDansFichier(argv[1], argv[2]);

    // ./nbFichiers ./
    //nbFichiers(argv[1]);
    
    return 0;

}