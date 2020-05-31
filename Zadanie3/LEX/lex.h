#include <iostream>

#include <fstream>

#ifndef CLASSLEX
#define CLASSLEX

#include <iostream>

using namespace std;
ofstream res;

enum type_of_lex { //перечислимый тип лексем
    lex_NULL, //0
    lex_AND,
    lex_OR,
    lex_NOT, //3
    lex_ID, //4
    lex_IF,
    lex_ELSE,
    lex_DO,
    lex_WHILE, //8
    lex_READ,
    lex_WRITE, //10
    lex_STRING,
    lex_BOOL,
    lex_INT, //13
    lex_TRUE,
    lex_FALSE, //15
    lex_PROGRAM, //16
    lex_SEMICOLON,
    lex_COMMA, //18
    lex_ASSIGN,
    lex_LPAREN,
    lex_RPAREN, //21
    lex_EQ,
    lex_LSS,
    lex_GTR, //24
    lex_LEQ,
    lex_REQ,
    lex_NEQ, //27
    lex_PLUS,
    lex_MINUS,
    lex_TIMES,
    lex_SLASH, //31
    lex_LBRAC,
    lex_RBRAC, //33
    lex_NUMB,
    lex_STR, //35
    lex_FIN, //36
    lex_BREAK, //37
	POLIZ_LABEL,
	POLIZ_ADDRESS,
	POLIZ_GO, //40
	POLIZ_FGO,
	POLIZ_DUP,
	lex_UNOM,
	lex_UNOP,
	PLUS_STRING, //45
	ASSIGN_STRING,
	LSS_STRING,
	GTR_STRING,
	EQ_STRING,
	NEQ_STRING, //50
	WRITE_STRING,
	WRITE_BOOL //52
};

class Lex { //клас лексем
    type_of_lex t_lex; //тип лексемы
    int v_lex; //номер в строчке массива лексем

    public:
        Lex(type_of_lex t = lex_NULL, int v = 0) {
            t_lex = t;
            v_lex = v;
        }
	
    type_of_lex get_type() {
        return t_lex;
    }

    int get_value() {
        return v_lex;
    }

    friend ostream & operator << (ostream & out, Lex l) {
		type_of_lex t = l.t_lex;
		switch (t) {
        case lex_AND:
            out << "AND ";
            break;
        case lex_OR:
            out << "OR ";
            break;
        case lex_NOT:
            out << "NOT ";
            break;
        case lex_IF:
            out << "IF ";
            break;
        case lex_ELSE:
            out << "ELSE ";
            break;
        case lex_DO:
            out << "DO ";
            break;
        case lex_WHILE:
            out << "WHILE ";
            break;
        case lex_READ:
            out << "READ ";
            break;
        case lex_WRITE:
            out << "WRITE ";
            break;
        case lex_STRING:
            out << "STRING ";
            break;
        case lex_BOOL:
            out << "BOOL ";
            break;
        case lex_INT:
            out << "INT ";
            break;
        case lex_TRUE:
            out << "TRUE ";
            break;
        case lex_FALSE:
            out << "FALSE ";
            break;
        case lex_PROGRAM:
            out << "PROGRAM ";
            break;
        case lex_SEMICOLON:
            out << "; ";
            break;
        case lex_COMMA:
            out << ", ";
            break;
        case lex_ASSIGN:
            out << "= ";
            break;
        case lex_LPAREN:
            out << "( ";
            break;
        case lex_RPAREN:
            out << ") ";
            break;
        case lex_EQ:
            out << "== ";
            break;
        case lex_LSS:
            out << "< ";
            break;
        case lex_GTR:
            out << "> ";
            break;
        case lex_LEQ:
            out << "<= ";
            break;
        case lex_REQ:
            out << ">= ";
            break;
        case lex_NEQ:
            out << "!= ";
            break;
        case lex_PLUS:
            out << "+ ";
            break;
        case lex_MINUS:
            out << "- ";
            break;
        case lex_TIMES:
            out << "* ";
            break;
        case lex_SLASH:
            out << "/ ";
            break;
        case lex_LBRAC:
            out << "{ ";
            break;
        case lex_RBRAC:
            out << "} ";
            break;
        case lex_ID:
            out << "ID" << l.v_lex << " ";
            break;
        case lex_FIN:
            out << "FINAL ";
            break;
        case lex_NUMB:
            out << "NUMB ";
            break;
        case lex_STR:
            out << "STR ";
            break;
        case lex_BREAK:
            out << "BREAK ";
            break;
		case lex_NULL:
            out << "NULL ";
            break;
		case POLIZ_LABEL:
			out << "POLIZ_LABEL"<< l.v_lex << " ";
			break;
		case POLIZ_ADDRESS:
			out << "POLIZ_ADDRESS" << l.v_lex << " ";
			break;
		case POLIZ_GO:
			out << "POLIZ_GO ";
			break;
		case POLIZ_FGO:
			out << "POLIZ_FGO ";
			break;	
		case lex_UNOM:
			out << "UNOM ";
			break;
		case lex_UNOP:
			out << "UNOP ";
			break;
		case POLIZ_DUP:
			out << "DUP ";
			break;
		case PLUS_STRING:
			out << "+S ";
			break;
		case ASSIGN_STRING:
			out << "=S ";
			break;
		case LSS_STRING:
			out << "<S ";
			break;
		case GTR_STRING:
			out << ">S ";
			break;
		case EQ_STRING:
			out << "==S ";
			break;
		case NEQ_STRING:
			out << "!= ";
			break;
		case WRITE_STRING:
			out << "WRITES ";
			break;
		case WRITE_BOOL:
			out << "WRITEB ";
		}
        return out;
    }
};

void record(type_of_lex t) { //функция для вывода лексемы в файл out
    switch (t) {
        case lex_AND:
            res << "lex_AND ";
            break;
        case lex_OR:
            res << "lex_OR ";
            break;
        case lex_NOT:
            res << "lex_NOT ";
            break;
        case lex_IF:
            res << "lex_IF ";
            break;
        case lex_ELSE:
            res << "lex_ELSE ";
            break;
        case lex_DO:
            res << "lex_DO ";
            break;
        case lex_WHILE:
            res << "lex_WHILE ";
            break;
        case lex_READ:
            res << "lex_READ ";
            break;
        case lex_WRITE:
            res << "lex_WRITE ";
            break;
        case lex_STRING:
            res << "lex_STRING ";
            break;
        case lex_BOOL:
            res << "lex_BOOL ";
            break;
        case lex_INT:
            res << "lex_INT ";
            break;
        case lex_TRUE:
            res << "lex_TRUE ";
            break;
        case lex_FALSE:
            res << "lex_FALSE ";
            break;
        case lex_PROGRAM:
            res << "lex_PROGRAM ";
            break;
        case lex_SEMICOLON:
            res << "lex_; ";
            break;
        case lex_COMMA:
            res << "lex_, ";
            break;
        case lex_ASSIGN:
            res << "lex_= ";
            break;
        case lex_LPAREN:
            res << "lex_( ";
            break;
        case lex_RPAREN:
            res << "lex_) ";
            break;
        case lex_EQ:
            res << "lex_== ";
            break;
        case lex_LSS:
            res << "lex_< ";
            break;
        case lex_GTR:
            res << "lex_> ";
            break;
        case lex_LEQ:
            res << "lex_<= ";
            break;
        case lex_REQ:
            res << "lex_>= ";
            break;
        case lex_NEQ:
            res << "lex_!= ";
            break;
        case lex_PLUS:
            res << "lex_+ ";
            break;
        case lex_MINUS:
            res << "lex_- ";
            break;
        case lex_TIMES:
            res << "lex_* ";
            break;
        case lex_SLASH:
            res << "lex_/ ";
            break;
        case lex_LBRAC:
            res << "lex_{ ";
            break;
        case lex_RBRAC:
            res << "lex_} ";
            break;
        case lex_ID:
            res << "lex_ID ";
            break;
        case lex_FIN:
            res << "lex_FINAL ";
            break;
        case lex_NUMB:
            res << "lex_NUMB ";
            break;
        case lex_STR:
            res << "lex_STR ";
            break;
        case lex_BREAK:
            res << "lex_BREAK ";
            break;
		case lex_NULL:
            res << "lex_NULL ";
            break;
		default:
			break;
    }
}

#endif
