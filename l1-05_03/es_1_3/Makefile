CC = g++
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.o random.o
	$(CC) ../../rand_gen/random.o main.o -o main.exe
main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)
random.o : ../../rand_gen/random.cpp ../../rand_gen/random.h
	$(CC) -c ../../rand_gen/random.cpp -o ../../rand_gen/random.o $(CFLAGS)



clean :
	rm *.o main.exe 
