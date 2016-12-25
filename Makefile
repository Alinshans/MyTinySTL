CXX=g++
CXXFLAGS =-std=c++11 -I/MyTinySTL/Test

#vpath %.h ./MyTinySTL
#vpath %.h ./MyTinySTL/Test

test: test.o
	$(CXX) -o $(CXXFLAGS) test.o
	
test.o: MyTinySTL/Test/test.cpp 
	$(CXX) -c test $(CXXFLAGS) test.cpp

clean:
	rm -rf *.o test
	
