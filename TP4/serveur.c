#include "calcul.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msg_id;

void raz_msg(int signal) {
	printf("Suppression de la file de message!\n");
	msgctl(msg_id,IPC_RMID,NULL);
	exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
	struct msg_struct msg;
	int i_sig;
	int result;
	key_t cle = ftok("serveur.c", 0);
	
		
	/* liberer la zone de messages sur reception de n'importe quel signal */
	for (i_sig = 0 ; i_sig < NSIG ; i_sig++) {
		signal(i_sig, raz_msg);
	}

	msg_id = msgget(cle, IPC_CREAT | 0666);
	if(msg_id == -1)
		perror("MSGET "), exit(1);

	printf("SERVEUR: pret!\n");
	while (1 == 1) {
		/* reception */
		if(msgrcv(msg_id, &msg, sizeof(struct infos),1L,0) <= 0) //message vide ou erreur
			fprintf(stderr,"Erreur msgrcv (ligne : %d).",__LINE__), exit(1);
		
		printf("SERVEUR: reception d'une requete de la part de: %d\n",msg.pid);
		/* preparation de la reponse */
		switch(msg.operation)
		{
			case '+' :
				result = msg.ope1 + msg.ope2;
				break;
			case '-':
				result = msg.ope1 - msg.ope2;
				break;
			case 'x' :
				result = msg.ope1 * msg.ope2;
				break;
			case '/': 
				result = msg.ope1 / msg.ope2;
				break;
			default :
				fprintf(stderr, "Erreur operateur non valide {%c}.\n",msg.operation);
				exit(1);
		}
		/* envoi de la reponse */
		printf("%d %c %d = %d\n", msg.ope1, msg.operation, msg.ope2, result);
		msg.type = msg.pid;
		msg.pid = getpid();
		msg.ope1 = result;
		if(msgsnd(msg_id, &msg, sizeof(struct infos),0) == -1)
			perror("msgsnd"),exit(1);
	}
	return EXIT_SUCCESS;
}

