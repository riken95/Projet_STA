#Fichiers
FILES_SERVEUR = misc.c server.c liste_trains.c
FILES_MAIN = main.c

#Exécutables
EXECUTABLE_SERVEUR = server
EXECUTABLE_MAIN = main


ENDFLAGS = -lncurses -lm
CC = gcc


all:
	@echo "Veuillez spécifier une règle !"
	@echo "Ex : server | server_exec | clean"

main:
	$(CC) $(FILES_MAIN) -o $(EXECUTABLE_MAIN) $(ENDFLAGS)

main_exec:
	./$(EXECUTABLE_MAIN)

server:
	$(CC) $(FILES_SERVEUR) -o $(EXECUTABLE_SERVEUR) $(ENDFLAGS) 

server_exec:
	./$(EXECUTABLE_SERVEUR)

clean:
	rm -f $(EXECUTABLE_SERVEUR)
	rm -f $(EXECUTABLE_MAIN)
	