CC=gcc
CFLAGS = -Wall
LIBS = -ldl 
#C_SRC = $(shell ls *.c)
C_SRC = server_procedures.c  server_unmarshall_req.c server.c server_marshal_res.c 
H_SRC = $(shell ls *.h)
H_SRC += ../remote_client/remoteop.h 
OBJS_FILES= $(C_SRC:.c=.o)
all: $(H_SRC) $(C_SRC)
	$(CC) -o server $(H_SRC) $(C_SRC) -ldl
