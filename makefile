FILES_SERVEUR = server.c
EXECUTABLE_SERVEUR = server
ENDFLAGS = -lm
CC = gcc


all:
	@echo "Veuillez spécifier une règle !"
	@echo "Ex : server | server_exec | clean"
server:
	$(CC) $(FILES_SERVEUR) -o $(EXECUTABLE_SERVEUR) $(ENDFLAGS)

server_exec:
	./$(EXECUTABLE_SERVEUR)

clean:
	rm -f $(EXECUTABLE_SERVEUR)