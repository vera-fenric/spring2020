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
    lex_BREAK //37
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
        out << '(' << l.t_lex << ',' << l.v_lex << ");";
        return out;
    }
};

void record(type_of_lex t) { //функция для вывода лексемы в файл res
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
    }
}

#endif
