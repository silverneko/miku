CC = clang++
CFLAG =  -std=c++11 -static

all: ./bin/miku ./bin/isolate

./bin/miku: ./src/main.cpp ./build/sandbox.o ./src/testsuite.cpp
	$(CC) -o ./bin/miku ./src/main.cpp ./build/sandbox.o $(CFLAG)

./build/sandbox.o: ./src/sandbox.cpp
	$(CC) -c ./src/sandbox.cpp -o ./build/sandbox.o $(CFLAG)

./bin/isolate:
	make -C ./isolate/
	cp ./isolate/isolate ./bin/isolate