CXX=g++
CXXFLAGS=-std=c++11 -Wall -Werror -pedantic -fsanitize=address -g
LDLIBS=-lm
OBJECTS=main.o Option.o Image.o Julia.o

.PHONY: clean

JuliaCPP: $(OBJECTS)
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
	rm $(OBJECTS) JuliaCPP &>/dev/null; true
