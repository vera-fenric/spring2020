#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
#include "../LEX/scan.h"

using namespace std;
 
class Parser {
  Lex           curr_lex;
  type_of_lex   c_type;
  int           c_val;
  Scanner       scan;
  stack < int > st_int;
  stack < type_of_lex >  st_lex;
  void P();
  void D();
  void D1();
  void B();
  void S();
  void E();
  void E1();
  void E2();
  void E3();
  void E4();
  void E5();
  void E6();
  void E7();
  void  declare ( type_of_lex type);

  void  get_next_lex () {
    curr_lex  = scan.get_lex();
    c_type    = curr_lex.get_type();
    c_val     = curr_lex.get_value();
  }
public:
  Parser ( const char *program ) : scan (program) { }
  void  analyze();
};
 
void Parser::analyze () {				//анализатор
	get_next_lex();						//считывает первую лексему
	P();
	if (c_type != lex_FIN)
		throw curr_lex;
}
 
void Parser::P () {						//P() проверяет,
	if (c_type != lex_PROGRAM)			//что программа начинается со слова program,
		throw curr_lex;
	get_next_lex ();
	if (c_type != lex_LBRAC)
		throw curr_lex;
	get_next_lex ();
	D(); 								//запускает обработку описаний
	B();								//и обработку операторов
	if (c_type != lex_RBRAC)
		throw curr_lex;
	get_next_lex();
}
 
void Parser::D () {						//D() обрабатывает блок описаний
    D1();								//D1() обрабатывает одно описание (т.е. фрагмент вида int i, j=5, k;)
	while (c_type == lex_SEMICOLON){	//для этого D() разделяет описание на фрагменты, разделяющиеся точкой с запятой
		get_next_lex ();
		D1();
	}
}
 
void Parser::D1 () {					//D1() обрабатывает одно описание
	type_of_lex t = c_type;				//t запоминает, какой тип идентификаторов в этой строке
	if ((t == lex_STRING) || (t == lex_BOOL) || (t==lex_INT)){		//строка может начинаться не со слова string, bool или int, тогда
		get_next_lex();												//мы должны переходить к обработке операторов, т.е. D1 ничего не делает
		if (c_type!=lex_ID)				//Если после string, bool или int идёт не идентификатор, то это ошибка
			throw curr_lex;
		while (c_type==lex_ID){			//пока идут идентификаторы через запятую
			st_int.push(c_val);			//мы их пихаем в стек
			get_next_lex();
			if ((c_type != lex_COMMA)&&(c_type != lex_SEMICOLON)&&(c_type != lex_ASSIGN))	//после идентификатора только , ; или =
				throw curr_lex;
			if (c_type==lex_ASSIGN){	//если = то нужно записать значение
				//тут надо шо-то делать с присвоением
				get_next_lex();
				get_next_lex();
			}
			if (c_type==lex_COMMA)		//если , идём дальше
				get_next_lex();
		}			
		declare(t);							//и если ; выходим из D1, сбрасывая стек
	}
}
 
void Parser::B () {						//B() обрабатывает операторы
	S();								//ей сразу на вход приходит первая лексема оператора, так что она ничего не проверяет
    while (c_type == lex_SEMICOLON) {
      get_next_lex ();
      S();
    }
}
 
void Parser::S () {						//S обрабатывает операторы
	if (c_type == lex_IF){				//оператор if
		get_next_lex();
		if (c_type != lex_LPAREN)
			throw curr_lex;
		get_next_lex();
		E();	
		if (c_type != lex_RPAREN)
			throw curr_lex;
		get_next_lex();
		S();
		if (c_type == lex_ELSE){
			get_next_lex();
			S();
		}else S();	//т.к. считали первую лексему за оператором после if
					//нужно обязательно запустить обработку этого оператора
					//иначе B не запустит S()		
	}else if (c_type == lex_READ){		//оператор read
		get_next_lex ();
		if (c_type != lex_LPAREN)
			throw curr_lex;
		get_next_lex ();
        if (c_type != lex_ID)
			throw curr_lex;
		get_next_lex ();
        if (c_type != lex_RPAREN)
			throw curr_lex;
		get_next_lex();
		
	}else if (c_type == lex_WRITE){		//оператор write
		get_next_lex();
		if (c_type!= lex_LPAREN)
			throw curr_lex;
		get_next_lex();
		E();
		while (c_type==lex_COMMA){
			get_next_lex();
			E();
		}
		if (c_type != lex_RPAREN)
			throw curr_lex;
		get_next_lex();
		
	}else if (c_type == lex_LBRAC){		//составной оператор
		get_next_lex();
		B();
		if (c_type!=lex_RBRAC)
			throw curr_lex;	
		get_next_lex();
		S();
		
	}else if (c_type == lex_WHILE){		//оператор while
		get_next_lex();
		if (c_type!=lex_LPAREN)
			throw curr_lex;
		get_next_lex();
		E();
		if (c_type!=lex_RPAREN)
			throw curr_lex;
		get_next_lex();
		S();
		
	}else if (c_type == lex_BREAK){		//оператор break
		get_next_lex();
		
	}else if (c_type == lex_SEMICOLON){ //пустой оператор
	
	}else{
		E();							//оператор-выражение
	}
}
  
void Parser::E () {
	E1();
	if ( c_type == lex_ASSIGN){
		get_next_lex();
		E();
	}
}
 
void Parser::E1 () {
	E2();
	if (c_type == lex_OR) {
		get_next_lex ();
		E1();
	}
}
 
void Parser::E2 () {
	E3();
	if ( c_type == lex_AND) {
		get_next_lex();
		E2();
	}
}
 
void Parser::E3 () {
	E4();
	if ((c_type ==lex_EQ)||(c_type==lex_LSS)||(c_type==lex_GTR)||(c_type==lex_LEQ)||(c_type==lex_REQ)||(c_type==lex_NEQ)){
		get_next_lex();
		E3();
	}	
}

void Parser::E4() {
	if (c_type == lex_MINUS)
		get_next_lex();
	E5();
	if ((c_type==lex_PLUS)||(c_type==lex_MINUS)){
		get_next_lex();
		E4();
	}		
}

void Parser::E5(){
	E6();
	if ((c_type==lex_TIMES)||(c_type==lex_SLASH)){
		get_next_lex();
		E5();
	}
}

void Parser::E6(){
	E7();
	if (c_type==lex_NOT){
		get_next_lex();
		E6();
	}
}

void Parser::E7(){
	if ( c_type == lex_ID ) {
		get_next_lex ();
	}else if ( c_type == lex_NUMB ) {
		get_next_lex ();
	}else if ( c_type == lex_TRUE ) {
		get_next_lex ();
	}else if ( c_type == lex_FALSE) {
		get_next_lex ();
	}else if (c_type == lex_STR){
		get_next_lex ();
	}else if ( c_type == lex_LPAREN ) {
		get_next_lex (); 
		E();
		if ( c_type != lex_RPAREN)
			throw curr_lex;
		get_next_lex();
	}
}


int main(){
	try{
		Parser p ("test.txt");
		p.analyze();
	}
	catch(Lex l){
		cout << "lexeme throwned" << endl;
		record(l.get_type());
	}
	catch (const char * p){
		cout << p << endl;
	}
	catch (...){
		cout << "Unknow situation";
	}
	cout << "Parser finished correctly"<< endl ;
	res.close();
}

void Parser::declare ( type_of_lex type ) {
	int i;
	while ( !st_int.empty()) {
		i = st_int.top();
		st_int.pop();
		if (TID[i].get_declare()) 
			throw "declared twice";
		else {
			TID[i].set_declare();
			TID[i].set_type(type);
		}
	}
}
