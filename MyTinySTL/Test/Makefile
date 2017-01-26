CXXFLAGS =-std=c++11

runtest: test.cpp
	$(CXX) -O2 -o test $(CXXFLAGS) test.cpp && ./test

clean:
	rm -f *.o runtest
