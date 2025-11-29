# 🚀 Mini-Bash 🖥️

Bienvenue sur le dépôt de mes TPs de **Programmation des Systèmes d'Exploitation** dans le cadre de ma troisième année en licence informatique à l'université d'*Avignon*.

Pour réaliser ces TPs, j'ai utilisé le langage ***C***.

Ci-dessous sont présentées quelques réalisations provenant de mes TPs.

--- 

## 🖥️ Mini-Bash

Le *Mini-Bash* (`MiniBash()`) agit comme un **interpréteur de commandes**, d'où le nom *Mini-Bash*.

Une fois l'exécutable lancé, pour pourrez entrer n'importe quelle commande Linux, comme dans un vrai interpréteur de commande.

⚠️ Ne traite pas les caractères suivants : `<`, `<<`, `>`, `|` !  
Par conséquent, ces exemples de commandes ne sont pas encore réalisables (car des questions du TP3 avaient pour but de régler ces problèmes mais je n'ai pas encore réussi à les implémenter) :  
- `ls > fichier`
- `ls >> fichier`
- `wc -l < fichier`
- `ls | wc -l`

Pour le quitter, utilisez le raccourci `CTRL + D` qui stoppe le programme.

---

## 🗣️ Affichage avec un mot de passe

La fonction `Affiche2(message, N)` affiche un `message` pendant `N` secondes ou **jusqu'à ce que l'utilisateur saissie le bon mot de passe**.

Le mot de passe par défaut est **password**. Il peut facilement être modifié au sein de la fonction `verifierPassword(sig)`.

---

## 📋 Exécution d'un fichier de commandes avec rapport chaque seconde

`ExecFileBatchReportENRCOMM` exécute en batch les commandes listées dans un fichier, tout en générant un **rapport d'exécution chaque seconde** grâce à la structure ***ENRCOMM***.  

Deux fichiers .txt contenant des commandes permettent de tester la bonne exécution des fonctions crées :  
- `commandes.txt` contient des commandes simples et instantanées.
- `wait.txt` vérifie le comportement des fonctions batch grâce à des ***sleep***.

---

## 📦 Gestion d’un fichier binaire partagé 

Ces trois fonctions manipulent un fichier binaire *base.dat* contenant 4 entiers :  

### 🔧 Init()

Crée ou réinitialise base.dat avec **4 entiers valant 100**.  

### ➖ ReserveWithoutLockf(n)

**Décrémente la valeur de l’entier numéro n** sans aucune protection concurrente.  
Cette fonction est volontairement **sujet aux conflits lorsque deux processus modifient la même case**.

### 🔒 ReserveWithLockf(n)

Même comportement, mais utilise ***lockf* pour verrouiller l’accès à la bonne zone du fichier**.  
Cette fonction **empêche deux processus d’éditer la même donnée simultanément** : idéal pour **visualiser la synchronisation entre terminaux**.

---

## 📁 Fonctions liées à lister des fichiers 

### 📂 Ls(repertoire)

Exécute la commande **ls sur un répertoire en créant un processus fils**.  

### 📄 LsDansFichier(repertoire, fichierResultat)

Même principe que Ls, mais **redirige la sortie dans un fichier**.  

---

## 🔢 Comptage du nombre de fichiers 

`nbFichiers(repertoire)` utilise un **pipe (`|`)** pour envoyer le résultat de la fonction `Ls(repertoire)` vers l'entrée de la fonction **wc -l**.  

Au total, il y a **2 processus et un *pipe* qui les relie.**  

La commande équivalente est `ls repertoire | wc -l`.  

La fonction retourne le nombre de fichiers présent dans le répertoire. 

Cela m'a permit de mieux comprendre comment toutes les commandes Linux sont implémentées, car la plupart des OS et des langages de programmation ont été crés avec le langage *C*.
