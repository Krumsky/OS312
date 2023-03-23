#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>                                   
#include <errno.h>                                                              
#include <signal.h>                                                                                                          
void rien(){};
int main(int argc, char *argv[])
{
    int count = 0;
    if(argc > 1)
    {
        count = atoi(argv[1]); //Atoi renovie 0 pour un chaine sans nombre
        if(count == 0)
            fprintf(stdout, "Attention le parametre (%s) n'est peut etre pas un nombre.\n"
                     "Le programme va commencer en partant de 0.\n", argv[1]);
        else if(count >= 100)
        {
            fprintf(stderr,"Le nombre donner est trop grands.\n" 
                           "Il doit etre en dessous de 100.\n");
            return 1;
        }    
    }
    pid_t pid = fork(); // creation du fils
	if (pid == -1) 
		perror("Fork error"), exit(1);
    if(pid) //Pere
    {                                            
        if(count%2) // c'est un nombre impaire le pere doit commencer
        {
            fprintf(stdout,"%d\n",count);
            kill(pid,SIGUSR1);
            count+=2;
        }
        else 
            count++; // C'est un nombre paire donc on rajoute 1 pour avoir le suivant
    }
    else //Fils
    {   
        pid = getppid();//On stokc le pid du pere pour lui envoyer dans la boucle for qui suit                       
        if(count%2 == 0) // c'est un nombre paire le fils doit commencer
        {
            fprintf(stdout,"%d\n",count);
            kill(getppid(),SIGUSR1);
            count+=2;
        }
        else 
            count++; // C'est un nombre impaire donc on rajoute 1 pour avoir le suivant
    }
    for(;count<=100; count+=2)
    {
        signal(SIGUSR1,rien);
        pause();
        fprintf(stdout,"%d\n",count);
        kill(pid,SIGUSR1);
    }
    return 0;
}