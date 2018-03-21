CXX = g++
FLAGS = -std=c++11 -m64 -Ofast -flto -march=native -funroll-loops
INCLUDE = -Iinclude
DEFINES = -DNDEBUG

all:
	rm -Rf bin
	$(CXX) $(FLAGS) $(INCLUDE) $(DEFINES) encoder/main.cpp -o encode
	$(CXX) $(FLAGS) $(INCLUDE) $(DEFINES) decoder/main.cpp -o decode
	mkdir bin
	mv encode bin
	mv decode bin
