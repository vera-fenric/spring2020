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

lex: LEX/main_lex.cpp lex.o
	g++ main_lex.o -o lex

lex.o: LEX/main_lex.cpp
	g++ $(CXXOPTIONS) -c LEX/main_lex.cpp

run_lex: LEX/main_lex.cpp lex.o lex
	./lex test.txt res_lex.txt

valgrind_lex: lex
	valgrind $(VALOPTIONS) ./lex test.txt res_lex.txt

par: PAR/main_par.cpp par.o 
	g++ main_par.o -o par

par.o: PAR/main_par.cpp
	g++ -c PAR/main_par.cpp

run_par: par
	./par

valgrind_par: par
	valgrind $(VALOPTIONS) ./par

