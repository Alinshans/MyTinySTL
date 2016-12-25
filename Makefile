CXXFLAGS = -Os -std=c++11

vpath %.h ./MyTinySTL
vpath %.h ./MyTinySTL/Test

all::test

test: test.cpp
    $(CXX) -o test $(CXXFLAGS) test.cpp

clean:
    rm -rf *.o main
