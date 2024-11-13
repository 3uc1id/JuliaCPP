CXX=g++
CXXFLAGS=-std=c++11 -Wall -Werror -pedantic -O2
LDLIBS=-lm

.PHONY: clean

JuliaCPP: main.o Option.o Image.o Julia.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

main.o: Option.o Image.o Julia.o
	$(CXX) $(CXXFLAGS) -c main.cpp -pthread

Julia.o: Image.o
	$(CXX) $(CXXFLAGS) -c Julia.cpp

Image.o: Image.h
	$(CXX) $(CXXFLAGS) -c Image.cpp

Option.o: Option.h
	$(CXX) $(CXXFLAGS) -c Option.cpp

clean:
	rm *.o JuliaCPP >>/dev/null
