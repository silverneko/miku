CC = clang++
CFLAGS =  -std=c++11 -static -Wall
SRC = ./src


all: ./bin/miku ./bin/isolate

./bin/miku: ./build/main.o ./build/sandbox.o
	$(CC) -o ./bin/miku ./build/main.o ./build/sandbox.o $(CFLAGS)

./build/main.o: ./src/main.cpp ./src/testsuite.cpp ./src/config.h
	$(CC) -o ./build/main.o -c ./src/main.cpp $(CFLAGS)
	
./build/sandbox.o: ./src/sandbox.cpp ./src/sandbox.h
	$(CC) -o ./build/sandbox.o -c ./src/sandbox.cpp $(CFLAGS)

./bin/isolate:
	make -C ./isolate/
	mv ./isolate/isolate ./bin/isolate

clean :
	rm ./build/*