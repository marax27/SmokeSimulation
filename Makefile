FLAGS=-Wall -Wextra -pedantic -std=c++17 -O1

TEST_DIR=Tests

sim: main.o Field3D.o SmokeSolver.o
	g++ $(FLAGS) main.o Field3D.o SmokeSolver.o -o sim

test:
	make -C $(TEST_DIR)

main.o: main.cpp SmokeSolver.hpp
	g++ $(FLAGS) -c main.cpp -o main.o

Field3D.o: Field3D.cpp Field3D.hpp base.hpp
	g++ $(FLAGS) -c Field3D.cpp -o Field3D.o

SmokeSolver.o: SmokeSolver.cpp SmokeSolver.hpp BaseSmokeSolver.hpp
	g++ $(FLAGS) -c SmokeSolver.cpp -o SmokeSolver.o

.PROXY:
clean:
	rm sim *.o
	make clean -C $(TEST_DIR)
