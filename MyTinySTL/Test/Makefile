CXXFLAGS =-std=c++11

ifeq ($(LANG),)
# for osx
runtest: test.cpp
	$(CXX) -o test $(CXXFLAGS) test.cpp && ./test
	
else
# for linux
runtest: test.cpp
	$(CXX) -O2 -o test $(CXXFLAGS) test.cpp && ./test

endif

clean:
	rm -f *.o runtest
