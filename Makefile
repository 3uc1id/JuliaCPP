CXX=g++
CXXFLAGS=-std=c++11 -Wall -Werror -pedantic -O2
LDLIBS=-lm

.PHONY: clean

JuliaCPP: main.o Option.o Image.o Julia.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: Option.h Image.h Julia.h
	$(CXX) $(CXXFLAGS) -c main.cpp -pthread
