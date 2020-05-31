CXXOPTIONS = -Wall -Werror
VALOPTIONS = --leak-check=full

all: lex par int

run: lex par int
	./lex test.txt res_lex.txt
	./par
	./int test.txt

valgrind: lex par int
	valgrind $(VALOPTIONS) ./lex test.txt res_lex.txt
	valgrind $(VALOPTIONS) ./par
	valgrind $(VALOPTIONS) ./int test.txt

clean:
	rm -rf *.o res_lex.txt

lex: LEX/main_lex.cpp main_lex.o
	g++ main_lex.o -o lex

main_lex.o: LEX/main_lex.cpp
	g++ $(CXXOPTIONS) -c LEX/main_lex.cpp

run_lex: LEX/main_lex.cpp lex.o lex
	./lex test.txt res_lex.txt

valgrind_lex: lex
	valgrind $(VALOPTIONS) ./lex test.txt res_lex.txt

par: PAR/main_par.cpp main_par.o 
	g++ main_par.o -o par

main_par.o: PAR/main_par.cpp
	g++ -c PAR/main_par.cpp

run_par: par
	./par

valgrind_par: par
	valgrind $(VALOPTIONS) ./par
	
int: INT/main_int.cpp main_int.o 
	g++ main_int.o -o int

main_int.o: INT/main_int.cpp
	g++ -c INT/main_int.cpp

run_int: int
	./int

valgrind_int: int
	valgrind $(VALOPTIONS) ./int

