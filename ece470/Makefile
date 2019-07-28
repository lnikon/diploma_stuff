INCLUDE=
CC=g++
#CFLAGS=-g -Wall $(INCLUDE)
CFLAGS=-O3 -Wall $(INCLUDE)
LIBS=-lm -lstdc++

CSOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp,%.o,${CSOURCES})
TARGETS=simulator

.PHONY: all clean
all: ${TARGETS}
simulator: ${OBJECTS}
	$(CC) $(CFLAGS) $(LIBS) -o $@ ${OBJECTS}

.cpp.o:
	$(CC) $(CFLAGS) -c $<
