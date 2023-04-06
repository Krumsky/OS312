#include "calcul.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>


int main(int argc, char const *argv[])
{
	int msg_id1; /*request*/
	int msg_id2; //reponses
	struct msg_struct msg;
	key_t key1; //clé des requetes
	key_t key2;
	if((key1 = ftok("serveur.c",0))==-1) { //file des requetes
		perror("prb ftok");
		exit(0);
	}
	if((key2 = ftok("client.c",0))==-1) { //file des reponses
                perror("prb ftok");
                exit(0);
        }

	if((msg_id1 = msgget(key1,0))==-1) {
		perror("prb msget");
		exit(0);
	}
	if((msg_id2 = msgget(key2,0))==-1) {
                perror("prb msget");
                exit(0);
        }
	printf( "Veuillez saisir la phrase : " );
        scanf( "%[^\n]", msg.message.buffeur);
	/* On prepare un message de type 1 à envoyer au serveur avec les
	 * informations necessaires */
	/* A COMPLETER */
	msg.type = 1;
	msg.message.pid = getpid();
	/* envoi du message */

	if(msgsnd(msg_id1,&msg,sizeof(struct message),0)==-1) {
		perror("prb msgsnd");
		exit(0);
	}

	/* reception de la reponse */

	if(msgrcv(msg_id2,&msg,sizeof(struct message),getpid(),0)==-1) {
		perror("prb msgrcv");
		exit(0);
	}
	printf("CLIENT: resultat recu depuis le serveur %d : %s\n",getpid(),msg.message.buffeur);
	return EXIT_SUCCESS;
}
