CC=gcc
CFLAGS = -Wall
LIBS = -lpcap
#C_SRC = remote_client_conn_setup.c remoteop.c 
C_SRC = $(shell ls *.c)
H_SRC = $(shell ls *.h)
#OBJS_FILES= $(C_SRC:.c=.o)
all: $(H_SRC) $(C_SRC)
	$(CC) -o pkt_parse $(H_SRC) $(C_SRC) $(LIBS) 
