# Makefile for 580 L1 assignment
main:  main.o
	g++ -Wall -pedantic -g -o main main.o

main.o: main.cpp
	g++ -Wall -pedantic -g -std=c++11 -c main.cpp	
		
clean:
	rm -f main.o main