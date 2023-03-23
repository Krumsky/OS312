#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#define TAILLE_MAX 20
int valeur;
int finJeu = 0;
void rien(){} //rien faire

void signalHandler(int sig)
{
    switch(sig)
    {
        case SIGUSR1: //Regarder dans le fichier
            fprintf(stdout,"Je suis %d et je vais essayer de ne pas mourir. \n", getpid());
            FILE *fp = fopen("Barillet", "r");
            if(fp == NULL)
                exit(1);
            char chaine[TAILLE_MAX];
            fgets(chaine,TAILLE_MAX, fp);
            fclose(fp);
            valeur = atoi(chaine);
            fprintf(stdout,"Fin de la lecture le num est %d.\n",valeur);
            break;
        case SIGUSR2: // Si un fils est mort go mourir en gros
            fprintf(stdout, "(%d) Je dois m'auto detruire malgré ma victoire. ;(  \n", getpid());
            exit(1);
            break;
        case SIGCHLD:
            finJeu = 1;
            break;
        default:
            break;
    }
}
/*
 * Fonction qui crée un nombre defini de processus fils en leurs associant chacun un numero
 * On sotck tout les PID des fils crée dans un tableau
 */
void forkNB(pid_t pidChild[], int *num, int nb_fils)
{
  int numero = 0;
  do{
    numero++;
    pidChild[numero-1] = fork();

    if (pidChild[numero-1] == -1) {
		fprintf(stderr, "Erreur : fork.\n");
		exit(1);
    }
  }while(pidChild[numero-1] != 0 && numero < nb_fils);

  if(pidChild[nb_fils-1] != 0) // Attribution du numero des processus, 0 est le pere.
    *num = 0;
  else
    *num = numero;
}

int main()
{
    int nb_fils = 6;
    pid_t *pidFils = malloc(sizeof(pid_t)*nb_fils);
    //////////////////////Ne pas oublier de free ce tableau //////////////////
    if(pidFils == NULL)
        fprintf(stderr, "Erreur malloc.\n"), exit(1);
    int numeroProcessus = 0;  // Num propre a chaque precessus 0 pour le pere, 1 pour le 1er fils
    forkNB(pidFils, &numeroProcessus, nb_fils);

    if(!numeroProcessus)
    {
        sleep(1); // temps d'attente de tout les processus soit en pause()
        int processMort = 0, i = 0;
        while(i < nb_fils)
        {
            fprintf(stdout,"(Pere) Envoie du signal SIGUSR1 à %d.\n", pidFils[i]);
            kill(pidFils[i], SIGUSR1);
            signal(SIGCHLD, signalHandler);
            signal(SIGUSR2, rien);
            pause();
            if(finJeu)
            {
                fprintf(stdout, "(Pere) Le process %d est mort.\n", i+1);
                processMort = i;
                i = nb_fils;
            }
            i++;  
        }

        for(i = 0; i<nb_fils;i++)
        {
            if(!finJeu)//mettre fin a tout les processus mais situation bizarre
            {
                kill(pidFils[i], SIGUSR2);
                fprintf(stdout, "(Pere) Extermination num = %d.\n", i+1);
            } 
            else if(i != processMort)
            {
                kill(pidFils[i], SIGUSR2);
                fprintf(stdout, "(Pere) Extermination num = %d.\n", i+1);
            } 
        }
        free(pidFils);
        exit(1); 
    }
    else
    {
        signal(SIGUSR1,signalHandler);
        signal(SIGUSR2,signalHandler);
        pause();
        if(valeur == numeroProcessus)
        {
            fprintf(stdout, "Oh non je meurt (numLu = %d, num = %d).\n", valeur, numeroProcessus);
            exit(1);
        }
        else 
        {
            fprintf(stdout ,"Oof pas mort (numLu = %d, num = %d).\n", valeur, numeroProcessus);
            kill(getppid(),SIGUSR2); // au suivant
            pause();
        }
    }
}