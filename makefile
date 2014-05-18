CC = clang++
CFLAGS =  -std=c++11 -static -Wall
SRC = ./src
HEADERS = ./src/config.h ./src/utils.h

all: | ./bin ./build ./bin/miku ./bin/isolate ./bin/batchjudge

./bin/miku: ./build/main.o ./build/sandbox.o ./build/testsuite.o ./build/server_io.o
	$(CC) -o ./bin/miku ./build/main.o ./build/sandbox.o ./build/testsuite.o ./build/server_io.o $(CFLAGS)

./build/main.o: ./src/main.cpp ./src/testsuite.h ./src/server_io.h $(HEADERS)
	$(CC) -o ./build/main.o -c ./src/main.cpp $(CFLAGS)
	
./build/sandbox.o: ./src/sandbox.cpp ./src/sandbox.h
	$(CC) -o ./build/sandbox.o -c ./src/sandbox.cpp $(CFLAGS)
	
./build/testsuite.o: ./src/testsuite.cpp ./src/testsuite.h ./src/sandbox.h $(HEADERS)
	$(CC) -o ./build/testsuite.o -c ./src/testsuite.cpp $(CFLAGS)
	
./build/server_io.o: ./src/server_io.cpp ./src/server_io.h $(HEADERS)
	$(CC) -o ./build/server_io.o -c ./src/server_io.cpp $(CFLAGS)
	
./bin/batchjudge: ./build/batchjudge.o ./build/sandbox.o
	$(CC) -o ./bin/batchjudge ./build/batchjudge.o ./build/sandbox.o $(CFLAGS)

./build/batchjudge.o: ./src/batchjudge.cpp ./src/sandbox.h
	$(CC) -o ./build/batchjudge.o -c ./src/batchjudge.cpp $(CFLAGS)

./bin/isolate: ./isolate/*
	make -C ./isolate/
	mv ./isolate/isolate ./bin/isolate

./bin:
	mkdir -p ./bin

./build:
	mkdir -p ./build

clean:
	rm -f -r ./build/*
