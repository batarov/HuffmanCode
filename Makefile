CXX = g++
FLAGS = -std=c++11 -Ofast -flto -march=native -funroll-loops
INCLUDE = -Iinclude

all:
	rm -Rf bin
	$(CXX) $(FLAGS) $(INCLUDE) encoder/main.cpp -o encode
	$(CXX) $(FLAGS) $(INCLUDE) decoder/main.cpp -o decode
	mkdir bin
	mv encode bin
	mv decode bin
