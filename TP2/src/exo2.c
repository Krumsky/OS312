#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void signalHandler(int sig)
{
    //ne rien faire
}

int main()
{
    pid_t minute = fork();
    if (minute == -1) {
		fprintf(stderr, "Erreur : fork.\n");
		return EXIT_FAILURE;
    }

    if(minute) //Pere Heure
    {
        fprintf(stdout,"Heure : 00 \n");
        int h = 0;
        while(1)
        {
            signal(SIGUSR1, signalHandler);
            pause();
            fprintf(stdout, "Heure : %d\n", ++h);
            if(h == 60)
                h = 0;
        }
    }
    else 
    {
        pid_t seconde = fork();
        if (seconde == -1) {
		    fprintf(stderr, "Erreur : fork.\n");
		    exit(1);
        }

        if(seconde) //fils minutes pere de secondes
        {
            sleep(1);
            fprintf(stdout,"Minute : 00\n");
            int min = 0;
            while(1)
            {
                signal(SIGUSR2, signalHandler);
                pause();
                fprintf(stdout, "min: %d\n", ++min);
                if(min == 60)
                {
                    min = 0;
                    kill(getppid(),SIGUSR1);
                }

            }
            
        }
        else //fils secondes 
        {
            sleep(2);
            fprintf(stdout,"Secondes : 00\n");
            int sec = 0;
            while(1)
            {
                alarm(1);
                signal(SIGALRM, signalHandler);
                pause();
                fprintf(stdout,"sec: %d\n",sec++);
                if(sec == 60)
                {
                    sec = 0;
                    kill(getppid(), SIGUSR2);
                }
            }
        }
    }
    return 0;
}