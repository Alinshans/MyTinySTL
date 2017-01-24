CXXFLAGS =-std=c++11

runtest: test.o
	$(CXX) -o $(CXXFLAGS) test.o

test.o: test.cpp
	$(CXX) -c $(CXXFLAGS) test.cpp

clean:
	rm -f *.o runtest
