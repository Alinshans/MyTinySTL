CXX=g++
CXXFLAGS =-std=c++11 -O2 -I/MyTinySTL/Test

vpath %.h ./MyTinySTL
vpath %.h ./MyTinySTL/Test

all: test

test.o: test.cpp
	$(CXX) -o test $(CXXFLAGS) test.cpp

clean:
	rm -rf *.o main

