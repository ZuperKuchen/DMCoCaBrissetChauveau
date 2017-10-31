INCLUDEDIR = -Itest/
CC = gcc
CFLAGS = -std=c99 -Wall -g
SRC = $(wildcard test/*.c)
OBJ = $(patsubst test/%.c, %.o, $(SRC))

EXECS = $(OBJ:.o= )

test: $(EXECS) 

$(EXECS) : $(OBJ) main.c 
	$(CC) $(CFLAGS) $(INCLUDEDIR) $@.o main.c -o $@ 

%.o :test/%.c
	$(CC) $(CFLAGS) -c -o $@ $<



.PHONY : clean

clean :
	rm *.o $(EXECS) 
