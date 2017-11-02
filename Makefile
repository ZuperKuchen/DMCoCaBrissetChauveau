INCLUDEDIR = -Itest/
CC = gcc
CFLAGS = -std=c99 -Wall -g
LIB = -lm
SRC = $(wildcard test/*.c)
OBJ = $(patsubst test/%.c, %.o, $(SRC))

EXECS = $(OBJ:.o= )

test: $(EXECS) 

$(EXECS) : $(OBJ) main.c 
	$(CC) $(CFLAGS) $(INCLUDEDIR) $@.o main.c -o $@ $(LIB)

%.o :test/%.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LIB)



.PHONY : clean

clean :
	rm *.o $(EXECS) *txt
