CXXOPTIONS = -Wall -Werror
VALOPTIONS = --leak-check=full

all: main

main: file.o tree.o main.o
	g++ file.o tree.o main.o -o prog

file.o: file.cpp
	g++ $(CXXOPTIONS) -c file.cpp

tree.o: tree.cpp
	g++ $(CXXOPTIONS) -c tree.cpp

main.o: main.cpp
	g++ $(CXXOPTIONS) -c main.cpp

run: main
	./prog

valgrind: main
	valgrind $(VALOPTIONS) ./prog

clean:
	rm -rf *.o prog
