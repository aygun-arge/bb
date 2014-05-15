#Compiler
CC=gcc

##FLAGS
#CFLAGS compiler flags, os flags will be appended to this
CFLAGS=-Wall
#LFLAGS linker flags, os flags will be appended to this
LFLAGS=

#PROFILER FLAGS
PROFILER=-pg# -v

#SOURCE FILES
SOURCES= spi.c

#NAME without extention
EXECUTABLE=spi

## DETECTING OS

#unix
EXT=
UNAME_S := $(shell uname -s)


# OBJECT USING BY FILES
OBJECTS=$(SOURCES:.c=.o)

help:
	@echo "Make"
	@echo "||\\      //||    //\\    || // ||==="
	@echo "|| \\    // ||   //  \\   ||//  ||___"
	@echo "||  \\  //  ||  //====\\  ||\\  ||"
	@echo "||   \\//   || //      \\ || \\ ||=== "
	@echo "usage:"
	@echo "make [args]"
	@echo "'all' : compile sources and make executable"
	@echo "'build' : same as 'all' but also run the executable"
	@echo "'rebuild' : remove the executable and all object files, compile sources and make executable"
	@echo "'run' : run the executable"
	@echo "'clean' : remove the executable and all object files"
	@echo "'clc' : remove all object files"

all:
	output

build:
	output run

run:
	./$(EXECUTABLE)$(EXT)

#rebuilding
rebuild:
	cleanignored output

#the linker
output:
	$(OBJECTS) $(CC) $(LFLAGS) $(OBJECTS) -o $(EXECUTABLE)$(EXT)
#object genaration
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@



#cleaning functions
clean:
	rm *.o $(EXECUTABLE)$(EXT)
cleanignored:
	-rm *.o $(EXECUTABLE)$(EXT)
clc:
	rm *.o
