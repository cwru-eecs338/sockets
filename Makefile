CC = clang
# -g : allows use of GNU Debugger
# -Wall : show all warnings
CFLAGS = -g -Wall
LIBS =
CSOURCE = client.c
SSOURCE = server.c
CLIENT = client
SERVER = server

all: $(CLIENT) $(SERVER)

client: $(CSOURCE)
	$(CC) $(LIBS) $(CFLAGS) -o $(CLIENT) $(CSOURCE)

server: $(SSOURCE)
	$(CC) $(LIBS) $(CFLAGS) -o $(SERVER) $(SSOURCE)

clean:
	@# Using the '@' sign suppresses echoing
	@# the line while the command is run
	rm -f $(CLIENT) $(SERVER)
