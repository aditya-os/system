CC=gcc
CFLAGS = -Wall
LIBS = -ldl 
#C_SRC = remote_client_conn_setup.c remoteop.c 
C_SRC = $(shell ls *.c)
#H_SRC = remote_client_conn_setup.h  remoteop.h 
H_SRC = $(shell ls *.h)
H_SRC += ../remote_client/remoteop.h 
OBJS_FILES= $(C_SRC:.c=.o)
all: $(H_SRC) $(C_SRC)
	$(CC) -o server $(H_SRC) $(C_SRC) -ldl
