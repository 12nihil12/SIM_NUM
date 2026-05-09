RPATH=../rand_gen/
CC = g++
ARMALIBS= -larmadillo -llapack -lblas
CFLAGS = -Wall -O3 --std=c++17 -I $(RPATH) 




main.exe : main.o random.o globe.o path.o population.o
	$(CC) $(RPATH)random.o main.o globe.o path.o population.o  -o main.exe $(ARMALIBS)


main.o : main.cpp population.h path.h globe.h $(RPATH)random.h
	$(CC) -c main.cpp -o main.o $(CFLAGS) 

globe.o: globe.cpp globe.h $(RPATH)random.h
	$(CC) -c globe.cpp -o globe.o $(CFLAGS)

path.o: path.cpp path.h globe.h $(RPATH)random.h
	$(CC) -c path.cpp -o path.o $(CFLAGS)

population.o: population.cpp population.h path.h globe.h $(RPATH)random.h
	$(CC) -c population.cpp -o population.o $(CFLAGS)




random.o : $(RPATH)random.cpp $(RPATH)random.h
	$(CC) -c $(RPATH)random.cpp -o $(RPATH)random.o $(CFLAGS)


clean :
	rm *.o main.exe 
clean_output :
	rm *.csv
	rm POPULATION/**
