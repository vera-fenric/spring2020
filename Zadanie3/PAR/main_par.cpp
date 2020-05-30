#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
#include "../LEX/scan.h"
#include "parser.h"

int main(){
	try{
		Parser p ("test.txt");
		p.analyze();
	}
	catch(Lex l){
		cout << "Lexeme throwned " << l << endl;
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