# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS =-Wall -std=c++11 -lGL -lstdc++ -lc -lm -lglut -lGLU $(shell pkg-config --cflags glfw3) 

LIBS =$(shell pkg-config --static --libs glfw3)

# ****************************************************
# Targets needed to bring the executable up to date
all :  Plot main prog clean_o clean_ghc run

Plot:
	$(CXX) -c Plot.cpp -o Plot.o $(CXXFLAGS) $(LIBS)  
main: 
	$(CXX) -c main.cpp -o main.o $(CXXFLAGS) $(LIBS)

prog :
	$(CXX) Plot.o main.o -o main $(CXXFLAGS) $(LIBS) 

run :
	./main	
clean_o : 
	rm -f *.o
clean_ghc: 	
	rm -f *.ghc












