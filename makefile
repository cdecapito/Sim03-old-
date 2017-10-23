CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

Sim03 : Sim03.o data.o process.o 
	$(CC) $(LFLAGS) Sim03.o data.o process.o -pthread -o Sim03

Sim03.o : Sim03.cpp data.cpp process.cpp readData.cpp simFuncs.cpp 
	$(CC) $(CFLAGS) Sim03.cpp 

data.o : data.cpp data.h
	$(CC) $(CFLAGS) data.cpp

process.o: process.cpp process.h
	$(CC) $(CFLAGS) process.cpp

clean: 
	\rm *.o Sim03