CXX=g++
CXX_FLAGS= -std=c++11 -Wall -g -O3
CPP_FILES := $(wildcard *.cc)
OBJ_FILES := $(CPP_FILES:.cc=.o)

%.o: %.cc
	$(CXX) -c $(CXX_FLAGS) -o $@ $< 

all: $(OBJ_FILES);
clean:
	rm -f *.o
