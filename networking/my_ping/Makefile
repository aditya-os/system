CC=gcc
CFLAGS = -Wall
#LIBS = -lpcap
C_SRC = $(shell ls *.c)
C_SRC += $(shell ls ../*.c)
H_SRC = $(shell ls *.h)
H_SRC += $(shell ls ../*.h)
#OBJS_FILES= $(C_SRC:.c=.o)
all: $(H_SRC) $(C_SRC)
	$(CC) -o myping $(H_SRC) $(C_SRC) $(LIBS) 
