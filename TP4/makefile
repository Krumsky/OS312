CC = gcc 
SRC = serveur.c client.c
BIN = $(patsubst %.c,%,$(SRC))
CFLAGS = -Wall 
.DEFAULT: aucun
aucun :
	@echo 'Usages : make {exo1 | exo2}'

.PHONY: exo1 exo2 clean 
exo1 : 
	$(CC) $(CFLAGS) $(MAKECMDGOALS)/$(word 1,$(SRC)) -o $(word 1,$(BIN))
	$(CC) $(CFLAGS) $(MAKECMDGOALS)/$(word 2,$(SRC)) -o $(word 2,$(BIN))
exo2 :
	$(CC) $(CFLAGS) $(MAKECMDGOALS)/$(word 1,$(SRC)) -o $(word 1,$(BIN))
	$(CC) $(CFLAGS) $(MAKECMDGOALS)/$(word 2,$(SRC)) -o $(word 2,$(BIN))
clean : 
	rm $(BIN)