CXX=g++
CXXFLAGS =-std=c++11 -I/MyTinySTL/Test

vpath %.h ./MyTinySTL
vpath %.h ./MyTinySTL/Test

all: test

test: test.o
	$(CXX) -o test test.o
	
test.o: test.cpp 
	$(CXX) -o test $(CXXFLAGS) test.cpp

clean:
	rm -rf *.o test
	
