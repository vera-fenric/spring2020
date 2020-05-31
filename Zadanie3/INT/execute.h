#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
#include "../LEX/scan.h"
#include "../PAR/parser.h"

class Table_string {
	int size;
	string t[50];
	char s[256];
	public:
	Table_string(){
		size=SL.count();
		for (int i=0; i<size; i++){	
			SL.get_str(s);
			t[i]=s;
		}
	};
	string operator[] (int i)const{
		return t[i];
	}
	void see(){
		for (int i=0; i<size; i++)
			cout << t[i];
	}
};

class Executer {
public:
    void execute ( vector <Lex> & poliz );
};
 
void Executer::execute ( vector <Lex> & poliz ) {
Table_string TS;
	
  Lex cur;
  stack <int> args;
  vector <string> str;
  int v, i, j, index = 0, size = poliz.size();
  int k=0;
  string s;
  while ( index < size ) {
	//  cout << index << " ";
    cur = poliz[index];
    switch (cur.get_type ()) {
 
    case lex_TRUE:
		args.push (1);
		break;
	case lex_FALSE:
		args.push (0);
		break;
	case lex_NUMB:
		args.push (cur.get_value());
		break;
	case POLIZ_ADDRESS:
	case POLIZ_LABEL:
		args.push (cur.get_value ());
		break;
	case lex_STR:
		args.push (str.size());
		str.push_back ((string)TS[cur.get_value()]); //нужно достать из списка констант
		break;
    case lex_ID:
		v = cur.get_value ();
		if (!TID[v].get_assign())
			throw "POLIZ: indefinite identifier";
		args.push ( TID[v].get_value () );
        break;
 
    case lex_NOT:
		i=args.top();
		args.pop();
		args.push( !i );
		break;
 
    case lex_OR:
      i=args.top();
	  args.pop(); 
      j=args.top();
	  args.pop();
      args.push ( j || i );
      break;
 
    case lex_AND:
      i=args.top(); args.pop();
      j=args.top(); args.pop();
      args.push ( j && i );
      break;
 
    case POLIZ_GO:
      i=args.top();
	  args.pop();
      index = i - 1;
      break;
 
    case POLIZ_FGO:
		i=args.top();
		args.pop();
		j=args.top();
		args.pop();
		if ( !j ) index = i-1;
		break;
 
    case lex_WRITE:
		i=args.top();
		args.pop();
		cout << i << endl;
		break;
	case WRITE_BOOL:
		i=args.top();
		args.pop();
		if (i)
			cout << "true" << endl;
		else
			cout << "false" << endl;
      break;
	case WRITE_STRING:
      i=args.top();
	  args.pop();
      cout << str[i] << endl;
      break;
	  
    case lex_READ:
      i=args.top();
	  args.pop();
      if ( TID[i].get_type () == lex_INT ) {
        cout << endl << "Input int value for " << TID[i].get_name ();
        cin >> k;
		TID[i].set_value (k);
		TID[i].set_assign ();
      }
      else if ( TID[i].get_type () == lex_STRING ) {
        cout << endl << "Input string value for " << TID[i].get_name () << endl;
        cin >> s;
		TID[i].set_value (str.size());
		TID[i].set_assign ();
		str.push_back(s);
      } else throw "Error in read";
      break;
 
    case lex_PLUS:
      i=args.top();
	  args.pop();
      j=args.top();
	  args.pop();
      args.push ( i + j );
      break;
 
    case lex_TIMES:
      i=args.top();
	  args.pop();
      j=args.top();
	  args.pop();
      args.push ( i * j );
      break;
 
    case lex_MINUS:
      i=args.top();
	  args.pop();
      j=args.top();
	  args.pop();
      args.push ( j - i );
      break;
 
    case lex_SLASH:
      i=args.top();
	  args.pop();
      j=args.top();
	  args.pop();
      if (!i) {
        args.push(j / i);
        break;
      }
      else
        throw "POLIZ:divide by zero";
 
    case lex_EQ:
      i=args.top();
	  args.pop();
      j=args.top();
	  args.pop();
      args.push ( i == j);
      break;
 
    case lex_LSS:
      i=args.top();
	  args.pop();
      j=args.top();
	  args.pop();
      args.push ( j < i);
      break;
 
    case lex_GTR:
      i=args.top(); args.pop();
      j=args.top(); args.pop();
      args.push ( j > i );
      break;
 
    case lex_LEQ:
      i=args.top(); args.pop();
      j=args.top(); args.pop();
      args.push ( j <= i );
      break;
 
    case lex_REQ:
      i=args.top(); args.pop();
      j=args.top(); args.pop();
      args.push ( j >= i );
      break;
 
    case lex_NEQ:
      i=args.top(); args.pop();
      j=args.top(); args.pop();
      args.push ( j != i );
      break;
 
    case lex_ASSIGN:
		i=args.top();
		args.pop();
		j=args.top();
		args.pop();
		TID[j].set_value(i);
		TID[j].set_assign();
		args.push(i);
		break;
	case lex_UNOM:
		i=args.top();
		args.pop();
		args.push ( -i );
		break;
	case lex_UNOP:
		break;
	case PLUS_STRING:
		i=args.top();
		args.pop();
		j=args.top();
		args.pop();
		args.push(str.size());
		str.push_back(str[j]+str[i]);		
		break;
	case LSS_STRING:
		i=args.top();
		args.pop();
		j=args.top();
		args.pop();
		args.push(str[j]<str[i]);
		break;
	case GTR_STRING:
		i=args.top();
		args.pop();
		j=args.top();
		args.pop();
		args.push(str[j]>str[i]);
		break;
	case EQ_STRING:
		i=args.top();
		args.pop();
		j=args.top();
		args.pop();
		args.push(str[j]==str[i]);
		break;
	case NEQ_STRING:
		i=args.top();
		args.pop();
		j=args.top();
		args.pop();
		args.push(str[j]!=str[i]);
		break;
	case ASSIGN_STRING:
		i=args.top();
		args.pop();
		j=args.top();
		args.pop();
		TID[j].set_value(str.size());
		args.push(str.size());
		str.push_back(str[i]);
		TID[j].set_assign();
		break;
	case POLIZ_DUP:
		i=args.top();
		args.push(i);
		break;
	case lex_SEMICOLON:
		while (!args.empty())
			args.pop();
		break;
    default:
      throw "POLIZ: unexpected elem";
    }
    ++index;
  };
  cout << "Finish of executing!!!" << endl;
}
 
class Interpretator {
  Parser   pars;
  Executer E;
public:
  Interpretator  (const char* program): pars (program) {  pars.analyze ();}
  void     interpretation ();
};
 
void Interpretator::interpretation () {
  E.execute ( pars.poliz );
}
