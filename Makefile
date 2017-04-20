CC = g++

default: kk

kk : kk.cpp
	$(CC) kk.cpp -o kk 
kk.o : kk.cpp kk.h
	$(CC)  -c kk.cpp
loop : kk.o
	$(CC) loop.cpp kk.o -o loop
clean :
	rm kk loop *.o