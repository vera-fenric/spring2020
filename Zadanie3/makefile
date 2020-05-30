CXXOPTIONS = -Wall -Werror
VALOPTIONS = --leak-check=full

all: lex par

run: lex par
	./lex test.txt res_lex.txt
	./par

valgrind: lex par
	valgrind $(VALOPTIONS) ./lex test.txt res_lex.txt
	valgrind $(VALOPTIONS) ./par

clean:
	rm -rf *.o res_lex.txt

lex: LEX/main.cpp lex.o
	g++ main.o -o lex

lex.o: LEX/main.cpp
	g++ $(CXXOPTIONS) -c LEX/main.cpp

run_lex: LEX/main.cpp lex.o lex
	./lex test.txt res_lex.txt

valgrind_lex: lex
	valgrind $(VALOPTIONS) ./lex test.txt res_lex.txt

par: par.o PAR/parser.cpp
	g++ parser.o -o par

par.o: PAR/parser.cpp
	g++ $(CXXOPTIONS) -c PAR/parser.cpp

run_par: par
	./par

valgrind_par: par
	valgrind $(VALOPTIONS) ./par

