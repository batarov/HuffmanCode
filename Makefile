CXX = g++
FLAGS = -std=c++11 -m64 -Ofast -flto -march=native -funroll-loops
INCLUDE = -Iinclude
DEFINES = -DNDEBUG
DEL_DIR = rm -R
MKDIR = mkdir -p
DISTDIR = bin

all:
	$(MKDIR) $(DISTDIR)
	$(CXX) $(FLAGS) $(INCLUDE) $(DEFINES) encoder/main.cpp -o $(DISTDIR)/encode
	$(CXX) $(FLAGS) $(INCLUDE) $(DEFINES) decoder/main.cpp -o $(DISTDIR)/decode

clean:
	$(DEL_DIR) $(DISTDIR)
