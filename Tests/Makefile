FLAGS=-Wall -Wextra -pedantic -std=c++17 -O1

INCLUDES=-I..  #where to look for header files

build:
	g++ $(FLAGS) Field3DTests.cpp ../Field3D.cpp $(INCLUDES) -o test

.PHONY:
clean:
	rm test a.out *.o
