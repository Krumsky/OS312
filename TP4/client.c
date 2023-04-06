#include "calcul.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char const *argv[])
{
	int msg_id;
	struct msg_struct msg;

	if (argc != 4) {
		printf("Usage: %s opereande1 {+|-|x|/} operande2. \n",argv[0]);
		return EXIT_FAILURE;
	}

	/* il faut eviter la division par zero */
	if(argv[2][0] == '/' && argv[3]== 0)
		fprintf(stderr, "Erreur division par 0.\n"), exit(1);

	key_t cle = ftok("serveur.c",0);
	/* ATTENTION : la file de messages doit avoir ete creee par le serveur. Il
	 * faudrait tester la valeur de retour (msg_id) pour verifier que cette
	 * creation a bien eu lieu. */
	
	msg_id = msgget(cle,0);
	if(msg_id == -1)
		perror("MSGET (creation) "), exit(1);
	printf("CLIENT %d: preparation du message contenant l'operation suivante:\
%d %c %d\n", getpid(), atoi(argv[1]), argv[2][0], atoi(argv[3]));

	/* On prepare un message de type 1 à envoyer au serveur avec les
	 * informations necessaires */
	msg.type = 1;
	msg.pid = getpid();
	msg.operation = argv[2][0];
	msg.ope1 = atoi(argv[1]);
	msg.ope2 = atoi(argv[3]);
	/* envoi du message */
	if(msgsnd(msg_id, &msg, sizeof(struct infos),0)== -1)
		perror("msgsnd"),exit(1);

	printf("On attend la reponse.\n");

	/* reception de la reponse */
	if( msgrcv(msg_id, &msg, sizeof(struct infos), getpid(), 0) == -1)
		perror("msgrcv"), exit(1);
		
	printf("CLIENT: resultat recu depuis le serveur %d : %d\n", msg.pid,msg.ope1);
	return EXIT_SUCCESS;
}
