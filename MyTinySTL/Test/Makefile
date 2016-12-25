CXX=g++
CXXFLAGS =-std=c++11

test: test.o
	$(CXX) -o $(CXXFLAGS) test.o
	
test.o: test.cpp 
	$(CXX) -c test $(CXXFLAGS) test.cpp

clean:
	rm -rf *.o test
