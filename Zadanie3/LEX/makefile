CXXOPTIONS = -Wall -Werror
VALOPTIONS = --leak-check=full

all: clean main

main: main.o
	g++ main.o -o lex

main.o: main.cpp
	g++ $(CXXOPTIONS) -c main.cpp

run: main
	./lex test.txt result.txt

valgrind: main
	valgrind $(VALOPTIONS) ./lex test.txt result.txt

clean:
	rm -rf *.o lex
