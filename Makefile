CXX=g++
CXX_FLAGS= -std=c++11 -Wall -g -O3
CPP_FILES := $(wildcard *.cc)
OBJ_FILES := $(CPP_FILES:.cc=.o)
AR_FILE := crypto.a

$(AR_FILE): $(OBJ_FILES)
	ar r $@ $^

%.o: %.cc
	$(CXX) -c $(CXX_FLAGS) -o $@ $<

all: $(AR_FILE)
clean:
	rm -f $(OBJ_FILES)
	rm -f $(AR_FILE)
