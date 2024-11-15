CXX=clang++
CXXFLAGS=-std=c++11 -Wall -Werror -pedantic -O2
LDLIBS=-lm
OBJECTS=main.o OptionParser.o Image.o Julia.o

.PHONY: clean

JuliaCPP: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.cpp OptionParser.o Image.o Julia.o
	$(CXX) $(CXXFLAGS) -c $< -pthread

Julia.o: Julia.cpp Julia.h Image.o
	$(CXX) $(CXXFLAGS) -c $<

Image.o: Image.cpp Image.h
	$(CXX) $(CXXFLAGS) -c $<

OptionParser.o: OptionParser.cpp OptionParser.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm $(OBJECTS) JuliaCPP &>/dev/null; true
