CC = g++

default: kk

kk : kk.cpp helpers.o
	$(CC) kk.cpp helpers.o -o kk 
helpers.o : helpers.cpp helpers.h
	$(CC) -c helpers.cpp
loop : helpers.o loop.cpp
	$(CC) loop.cpp helpers.o -o loop
clean :
	rm kk loop *.o