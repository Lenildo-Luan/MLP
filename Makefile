CC = g++

all: 
	$(CC) -std=c++11 -O3 ./src/main.cpp ./src/readData.cpp ./src/Util.cpp -o mlp

clean:
	rm mlp