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
  bool flag_assign;
  int value_assign;
  Lex           curr_lex;
  type_of_lex   c_type;
  int           c_val;
  Scanner       scan;
  stack < int > st_int;
  stack < type_of_lex >  st_lex;
  stack < int > st_break;
  stack < int > st_if_true;
  stack < int > st_if_false;
  void P();
  void D();
  void D1();
  void B(int);
  void S(int);
  void E();
  void E1();
  void E2();
  void E3();
  void E4();
  void E5();
  void E6();
  void E7();
  void  declare ( type_of_lex type);
  void  check_id ();
  void  check_op ();
  void  check_not ();
  void  check_uno();
  void  eq_type ();
  void  eq_bool ();
  void  check_id_in_read ();

  void  get_next_lex () {
    curr_lex  = scan.get_lex();
    c_type    = curr_lex.get_type();
    c_val     = curr_lex.get_value();
  }
public:
	vector <Lex> poliz;
	Parser ( const char *program ) : scan (program), flag_assign(false){ }
	void  analyze();
};

void Parser::analyze () {				//анализатор
	get_next_lex();						//считывает первую лексему
	int i=0;
	P();
	if (c_type != lex_FIN)
		throw curr_lex;
	cout << endl << "Poliz output:" << endl;
	for (Lex l : poliz) 
		cout << "(" << i++ << ")" << l << ' ';
	cout << endl << "Poliz done" << endl;
}
 
void Parser::P () {						//P() проверяет,
	if (c_type != lex_PROGRAM)			//что программа начинается со слова program,
		throw curr_lex;
	get_next_lex ();
	if (c_type != lex_LBRAC)
		throw curr_lex;
	get_next_lex ();
	D(); 								//запускает обработку описаний
	B(-1);								//и обработку операторов
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
				poliz.push_back (Lex (POLIZ_ADDRESS, st_int.top()));
				st_lex.push(t);
				get_next_lex();
				E1();
				eq_type();
				poliz.push_back (Lex (lex_ASSIGN));
			}
			if (c_type==lex_COMMA)		//если , идём дальше
				get_next_lex();
		}			
		declare(t);						//и если ; выходим из D1, сбрасывая стек
	}
}
 
void Parser::B (int p=-1) {						//B() обрабатывает операторы
	S(p);
    while (c_type == lex_SEMICOLON) {
		get_next_lex ();
		while (!st_lex.empty())
			st_lex.pop();
		S(p);
    }
}

 
void Parser::S (int p=-1) {						//S обрабатывает операторы
	int p1, p2;
	if (c_type == lex_IF){				//оператор if
		get_next_lex();
		if (c_type != lex_LPAREN)
			throw curr_lex;
		get_next_lex();
		E1();
		eq_bool ();
		p1 = poliz.size();
		poliz.push_back (Lex());
		poliz.push_back (Lex(POLIZ_FGO));
		if (c_type != lex_RPAREN)
			throw curr_lex;
		get_next_lex();
		
		while (!st_if_true.empty()){
			poliz[st_if_true.top()]=Lex(POLIZ_LABEL,poliz.size());
			st_if_true.pop();
		}
		
		if (c_type == lex_LBRAC){		//составной оператор Внутри 'if-then'
			get_next_lex();
			B(p);
			if (c_type!=lex_RBRAC)
				throw curr_lex;
		} else {
			S(p);
			if (c_type!=lex_SEMICOLON)
				throw curr_lex;
		}
		get_next_lex();

		if (c_type == lex_ELSE){
			p2 = poliz.size ();
			poliz.push_back ( Lex() );
			poliz.push_back (Lex(POLIZ_GO));
			poliz[p1] = Lex(POLIZ_LABEL, poliz.size());

			while (!st_if_false.empty()){
				poliz[st_if_false.top()]=Lex(POLIZ_LABEL,poliz.size());
				st_if_false.pop();
			}
			get_next_lex();
			S(p);
			poliz[p2] = Lex(POLIZ_LABEL, poliz.size());
		}else{
			while (!st_if_false.empty()){
				poliz[st_if_false.top()]=Lex(POLIZ_LABEL,poliz.size());
				st_if_false.pop();
			}
			poliz[p1] = Lex(POLIZ_LABEL, poliz.size());
			S(p);
		}
	}else if (c_type == lex_READ){		//оператор read
		get_next_lex ();
		if (c_type != lex_LPAREN)
			throw curr_lex;
		get_next_lex ();
        if (c_type != lex_ID)
			throw curr_lex;
		check_id_in_read();
        poliz.push_back (Lex( POLIZ_ADDRESS, c_val));
		get_next_lex ();
        if (c_type != lex_RPAREN)
			throw curr_lex;
		poliz.push_back (Lex (lex_READ));
		get_next_lex();
		
	}else if (c_type == lex_WRITE){		//оператор write
		get_next_lex();
		if (c_type!= lex_LPAREN)
			throw curr_lex;
		get_next_lex();
		E1();
		poliz.push_back (Lex(lex_WRITE));
		while (c_type==lex_COMMA){
			get_next_lex();
			E1();
			poliz.push_back (Lex(lex_WRITE));
		}
		if (c_type != lex_RPAREN)
			throw curr_lex;
		get_next_lex();
		
	}else if (c_type == lex_LBRAC){		//составной оператор
		get_next_lex();
		B(p);
		if (c_type!=lex_RBRAC)
			throw curr_lex;
		get_next_lex();
		S();
		
	}else if (c_type == lex_WHILE){		//оператор while
		p1 = poliz.size();
		get_next_lex();
		if (c_type!=lex_LPAREN)
			throw curr_lex;
		get_next_lex();
		E1();
		eq_bool();
		if (c_type!=lex_RPAREN)
			throw curr_lex;
		p2 = poliz.size();
		poliz.push_back ( Lex() );
		poliz.push_back (Lex(POLIZ_FGO));
		get_next_lex();
	
		if (c_type == lex_LBRAC){		//составной оператор Внутри 'while-do'
			get_next_lex();
			B(p1);
			if (c_type!=lex_RBRAC)
				throw curr_lex;
		} else {
			S(p1);
			if (c_type!=lex_SEMICOLON)
				throw curr_lex;
		}			
		poliz.push_back (Lex(POLIZ_LABEL, p1));
		poliz.push_back (Lex(POLIZ_GO));
		poliz[p2] = Lex(POLIZ_LABEL, poliz.size());
		while (!st_break.empty()){
			p2=st_break.top();
			poliz[p2] = Lex(POLIZ_LABEL, poliz.size());
			st_break.pop();
		}
		get_next_lex();
		S();
			
	}else if (c_type == lex_BREAK){		//оператор break
		if (p==-1)
			throw curr_lex;
		get_next_lex();
		st_break.push (poliz.size());
		poliz.push_back (Lex());
		poliz.push_back (Lex(POLIZ_GO));
		
	}else if (c_type == lex_SEMICOLON){ //пустой оператор
	
	}else if (c_type == lex_ID)
		E();
}
  
void Parser::E () {
	if (c_type==lex_ID){
		flag_assign=true;
		value_assign=c_val;
		check_id ();
		st_lex.push (TID[c_val].get_type());
		get_next_lex();
		if (c_type == lex_ASSIGN) {
			poliz.push_back (Lex (POLIZ_ADDRESS, value_assign)); 
			flag_assign=false;
			get_next_lex ();
			E();
			eq_type();
			poliz.push_back (Lex (lex_ASSIGN));
		}else{
			E1();
		}
	}else
		E1();
}
 
void Parser::E1 () {
	E2();
	while (c_type == lex_OR) {			//ленивые вычисления
		poliz.push_back(Lex(POLIZ_DUP));
		poliz.push_back(Lex(POLIZ_LABEL,poliz.size()+4));
		poliz.push_back(Lex(POLIZ_FGO));
		st_if_true.push(poliz.size());
		poliz.push_back(Lex());
		poliz.push_back(Lex(POLIZ_GO));

		st_lex.push (c_type);
		get_next_lex ();
		E2();
		check_op();
	}
}
 
void Parser::E2 () {
	E3();
	while ( c_type == lex_AND) {			//ленивые вычисления
		poliz.push_back(Lex(POLIZ_DUP));
		st_if_false.push(poliz.size());
		poliz.push_back(Lex());
		poliz.push_back(Lex(POLIZ_FGO));

		st_lex.push (c_type);
		get_next_lex();
		E3();
		check_op();
	}
}
 
void Parser::E3 () {
	E4();
	while ((c_type ==lex_EQ)||(c_type==lex_LSS)||(c_type==lex_GTR)||(c_type==lex_LEQ)||(c_type==lex_REQ)||(c_type==lex_NEQ)){
		st_lex.push (c_type);
		get_next_lex();
		E4();
		check_op();
	}	
}

void Parser::E4() {
	if ((c_type == lex_MINUS)&&(!flag_assign)){
		get_next_lex();
		E5();
		check_uno();
		poliz.push_back (Lex (lex_UNOM));
		while ((c_type==lex_PLUS)||(c_type==lex_MINUS)){
			st_lex.push (c_type);
			get_next_lex();
			E5();
			check_op();
		}
		
	if ((c_type == lex_PLUS)&&(!flag_assign)){
		get_next_lex();
		E5();
		check_uno();
		poliz.push_back (Lex (lex_UNOP));
		while ((c_type==lex_PLUS)||(c_type==lex_MINUS)){
			st_lex.push (c_type);
			get_next_lex();
			E5();
			check_op();
		}
	}
	}else{
		E5();
		while ((c_type==lex_PLUS)||(c_type==lex_MINUS)){
			st_lex.push (c_type);
			get_next_lex();
			E5();
			check_op();
		}
	}		
}

void Parser::E5(){
	E6();
	while ((c_type==lex_TIMES)||(c_type==lex_SLASH)){
		st_lex.push (c_type);
		get_next_lex();
		E6();
		check_op();
	}
}

void Parser::E6(){
	if ((c_type==lex_NOT)&&(!flag_assign)){
		//st_lex.push (c_type);
		get_next_lex();
		E7();
		check_not();
	} else E7();
}

void Parser::E7(){
	if (flag_assign){
		poliz.push_back (Lex (lex_ID, value_assign));
		flag_assign=false;
		return;		
	}
	
	if ( c_type == lex_ID ) {
		check_id();
		poliz.push_back (Lex (lex_ID, c_val));
		get_next_lex ();
	}else if ( c_type == lex_NUMB ) {
		st_lex.push ( lex_INT );
		poliz.push_back ( curr_lex );
		get_next_lex ();
	}else if ( c_type == lex_TRUE ) {
		st_lex.push ( lex_BOOL );
		poliz.push_back (Lex (lex_TRUE, 1) );
		get_next_lex ();
	}else if ( c_type == lex_FALSE) {
		st_lex.push ( lex_BOOL );
		poliz.push_back (Lex (lex_FALSE, 0) );
		get_next_lex ();
	}else if (c_type == lex_STR){
		st_lex.push ( lex_STRING );
		poliz.push_back ( curr_lex );
		get_next_lex ();		
	}else if ( c_type == lex_LPAREN ) {
		get_next_lex (); 
		E1();
		if ( c_type != lex_RPAREN)
			throw curr_lex;
		get_next_lex();
	}
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


void Parser::check_id () {
	if ( TID[c_val].get_declare() )
		st_lex.push ( TID[c_val].get_type());
	else 
		throw "not declared";
}
 
void Parser::check_op () {
	type_of_lex o1, o2, op;
 
	o2 = st_lex.top();
	st_lex.pop();
	op = st_lex.top();
	st_lex.pop();
	o1 = st_lex.top();
	st_lex.pop();
	if (op==lex_PLUS)
		if ((o1==o2)&&(o1==lex_STRING))
			st_lex.push(lex_STRING);
		else if ((o1==o2)&&(o1==lex_INT))
			st_lex.push(lex_INT);
		else
			throw "Wrong types +";
	if ((op==lex_MINUS)||(op==lex_TIMES)||(op==lex_SLASH))
		if ((o1==o2)&&(o1==lex_INT))
			st_lex.push(lex_INT);
		else
			throw "Wrong types - * /";
	if ((op ==lex_EQ)||(op==lex_LEQ)||(op==lex_REQ)||(op==lex_NEQ))
		if ((o1==o2)&&(o1==lex_STRING))
			st_lex.push(lex_BOOL);
		else if ((o1==o2)&&(o1==lex_INT))
			st_lex.push(lex_BOOL);
		else
			throw "Wrong types == != < >";
	if ((op==lex_LSS)||(op==lex_GTR))
		if ((o1==o2)&&(o1==lex_INT))
			st_lex.push(lex_BOOL);
		else
			throw "Wrong types >= <=";
	if ((op==lex_AND)||(op==lex_OR))
		if ((o1==o2)&&(o1==lex_BOOL))
			st_lex.push(lex_BOOL);
		else
			throw "Wrong types and or";	
	poliz.push_back (Lex (op));
}
 
void Parser::check_not () {
	if (st_lex.top() != lex_BOOL)
		throw "Wrong type not";
	else  
		poliz.push_back (Lex (lex_NOT));
}

void Parser::check_uno () {
	if (st_lex.top() != lex_INT)
		throw "Wrong type uno";
}
 
void Parser::eq_type () {
	type_of_lex t;
	t=st_lex.top();
	st_lex.pop();
	if ( t != st_lex.top())
		throw "Wrong types in assign";
	st_lex.pop();
}
 
void Parser::eq_bool () {
	if ( st_lex.top() != lex_BOOL )
		throw "Expression is not boolean";
	st_lex.pop();
}
 
void Parser::check_id_in_read () {
	if ( !TID [c_val].get_declare() )
		throw "Not declared";
	if ( TID [c_val].get_type() ==lex_BOOL)
		throw "Cannot read bool type";
}
