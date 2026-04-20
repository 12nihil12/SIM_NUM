RPATH= ../rand_gen/
SIMPATH=./NSL_SIMULATOR/SOURCE/
CC = g++
CFLAGS = -O3 --std=c++14 -I$(RPATH) -I$(SIMPATH)
AFLAGS = -larmadillo

all: main.exe
main.exe : $(SIMPATH)system.o main.o $(SIMPATH)particle.o $(RPATH)random.o
	$(CC) $(SIMPATH)system.o $(SIMPATH)particle.o $(RPATH)random.o main.o -o main.exe $(AFLAGS)
main.o : main.cpp $(SIMPATH)system.h $(SIMPATH)particle.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)
$(SIMPATH)system.o : $(SIMPATH)system.cpp $(SIMPATH)system.h $(RPATH)random.h $(SIMPATH)particle.h
	$(CC) -c $(SIMPATH)system.cpp -o $(SIMPATH)system.o $(CFLAGS)
$(SIMPATH)particle.o : $(SIMPATH)particle.cpp $(SIMPATH)particle.h
	$(CC) -c $(SIMPATH)particle.cpp -o $(SIMPATH)particle.o $(CFLAGS)
$(RPATH)random.o : $(RPATH)random.cpp $(RPATH)random.h
	$(CC) -c $(RPATH)random.cpp -o $(RPATH)random.o $(CFLAGS)
clean :
	rm *.o main.exe
remove:
	rm ../OUTPUT/*.*
	rm ../OUTPUT/CONFIG/*.*