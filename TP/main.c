#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {

    // ./ligne_to_argv "ls -l"
    //char ** commande = Ligne2ARGV(argv[1]);
    //AfficheArgv(commande);

    // ./argv_to_ligne "ls -l"
    //char * ligne = Argv2Ligne(commande);
    //printf("%s\n", ligne);

    // ./execute "ls -l"
    //int retour = Execute(commande);
    //printf("%d\n", retour);

    // ./mini_bash
    //MiniBash();

    // ./file_to_tab_argv commandes.txt
    //char *** TabArgv;
    //int nbCommandesLues;
    //TabArgv = File2TabArgv(argv[1], &nbCommandesLues);
    //AfficheTabArgv(TabArgv);
    //printf("%d\n", nbCommandesLues);

    // ./exec_file commandes.txt
    //ExecFile(argv[1]);

    // ./execute_batch "sleep 10"
    //char ** commande = Ligne2ARGV(argv[1]);
    //ExecuteBatch(commande);

    // ./exec_file_batch wait.txt
    //ExecFileBatch(argv[1]);

    // ./file_to_tab_com commandes.txt
    //ENRCOMM * TabCom;
    //int nbCommandesLues;
    //TabCom = File2TabCom(argv[1], &nbCommandesLues);
    //AfficheTabENRCOMM(TabCom, nbCommandesLues);

    // ./exec_file_enrcomm commandes.txt
    //ExecFileENRCOMM(argv[1]);

    // ./exec_file_enrcomm_batch commandes.txt
    //ExecFileBatchENRCOMM(argv[1]);

    // ./exec_file_enrcomm_batch_limite commandes.txt
    ExecFileBatchLimite(argv[1], atoi(argv[2]));
    
    return 0;

}