GCC=gcc
FLAGS= -g
TOP_LEVEL= /home/pmlaforest/projets/chat2utilisateurs
SRC = src
DEST = $(TOP_LEVEL)/bin/

all: srv client conv_window

conv_window: convo.o str2.o
	$(GCC) $(FLAGS) $(DEST)convo.o $(DEST)str2.o -o $(DEST)conv_window

client: cli_main.o cli_messages.o fifo.o dynarray.o str2.o  tcp_client.o
	$(GCC) $(FLAGS) $(DEST)cli_main.o $(DEST)str2.o $(DEST)fifo.o $(DEST)cli_messages.o $(DEST)dynarray.o $(DEST)tcp_client.o -o $(DEST)client

srv:	srv_main.o str2.o info_client_struct.o srv_actions.o srv_bd.o
	$(GCC) $(FLAGS) $(DEST)srv_main.o $(DEST)str2.o $(DEST)srv_actions.o $(DEST)info_client_struct.o $(DEST)srv_bd.o -o $(DEST)srv -lpthread 

srv_main.o: srv_main.c srv_main.h  structs_clients.h
	$(GCC) $(FLAGS) -c srv_main.c -o $(DEST)srv_main.o

srv_actions.o: srv_actions.c srv_actions.h
	$(GCC) $(FLAGS) -c srv_actions.c -o $(DEST)srv_actions.o

srv_bd.o: srv_bd.c srv_bd.h
	$(GCC) $(FLAGS) -c srv_bd.c -lsqlite3 -o $(DEST)srv_bd.o 

cli_main.o: cli_main.c cli_main.h structs_clients.h str2.h
	$(GCC) $(FLAGS) -c cli_main.c -o $(DEST)cli_main.o

cli_messages.o: cli_messages.c cli_messages.h
	$(GCC) $(FLAGS) -c cli_messages.c -o $(DEST)cli_messages.o

fifo.o: fifo.c fifo.h
	$(GCC) $(FLAGS) -c fifo.c -o $(DEST)fifo.o

str2.o: str2.c
	$(GCC) $(FLAGS) -c str2.c -o $(DEST)str2.o

convo.o: conv_window.c structs_clients.h str2.h
	$(GCC) $(FLAGS) -c conv_window.c -o $(DEST)convo.o 

info_client_struct.o: info_client_struct.c info_client_struct.h
	$(GCC) $(FLAGS) -c info_client_struct.c -o $(DEST)info_client_struct.o

dynarray.o: dynarray.c dynarray.h
	$(GCC) $(FLAGS) -c dynarray.c -o $(DEST)dynarray.o

tcp_client.o: tcp_client.c tcp_client.h
	$(GCC) $(FLAGS) -c tcp_client.c -o $(DEST)tcp_client.o

clean :
	rm $(DEST)* *~
