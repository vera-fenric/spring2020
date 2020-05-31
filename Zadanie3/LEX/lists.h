#ifndef LISTS
#define LISTS

#include "lex.h"

#include "ident.h"

#include "lists.h"

class digit_list { //класс FIFO список чисел из программы (нужно для работы следующих этапов)
    struct digit {
        int dig;
        digit * next;
    };
    digit * first;
    digit * last;

    public:
        digit_list() {
            first = new digit;
            last = first;
            first->next=NULL;
        }

        ~digit_list() { //деструктор освобождающий всю память, если вдруг этого не произошло
            digit * p;
            if (first!=NULL){
                    while (first != last) {
                    p = first;
                    first = first -> next;
                    delete p;
                }
                delete first;
            }
        }

    void add_dig(const int d) { //добавление числа в список
        last -> dig = d;
        last -> next = new digit;
        last = last -> next;
		last ->next = NULL;
    }

    int get_dig(int & a) { //получение числа из списка (значение записывается в параметр)
        digit * p;
        if ((first == last) || (first == NULL))
            return 0;
        p = first;
        a = p -> dig;
        first = first -> next;
        delete p;
        return 1;
    }

    void print_digits() { //метод, выводящий все числа из списка, не разрушая его
        digit * p = first;
        while ((p != last)&&(p!=NULL)) {
            cout << p -> dig << ' ';
            p = p -> next;
        }
    }

    void print_ids() { //метод, специально для списка идентификаторов, который так же типа digit_list
        digit * p = first;
        while ((p != last)&&(p!=NULL)) {
            res << TID[p -> dig].get_name() << ' ';
            p = p -> next;
        }
    }
};


class str_list { //класс FIFO список строк из программы (нужно для работы следующих этапов)
    struct str {
        char * s;
        str * next;
    };
    str * first;
    str * last;

    public:
        str_list() {
            first = new str;
            last = first;
            first->s = NULL;
            first->next = NULL;
        }

        ~str_list() { //деструктор освобождающий всю память, если вдруг этого не произошло
            str * p;
            while ((first!=NULL)&&(first != last)) {
				p = first;
				first = first -> next;
				if (p -> s != NULL)
					delete[] p -> s;
				delete p;
			}
            if (first!=NULL){
				if (first -> s != NULL)
					delete[] first -> s;
				delete first;
			}
        }

    void add_str(const char * d) { //добавление строки в список
        last -> s = new char[strlen(d)+1];
        strcpy(last -> s, d);
        last -> next = new str;
        last = last -> next;
	last -> s = NULL;
	last -> next = NULL;
    }

    int get_str(char * a) { //получение строки из списка (значение записывается в параметр)
        str * p;
        if ((first == last)||(first==NULL)) {
            return 0;
        }
		p = first;
        strcpy(a, p -> s);
        first = first -> next;
		delete p->s;
        delete p;
        return 1;
    }

    void print_strs() { //вывод всех строк из списка, не разрушая его
        str * p = first;
        while ((p != last)&&(p!=NULL)) {
            res << '\"' << p -> s << '\"' << '\n';
            p = p -> next;
        }
    }
	int count(){
		str * p = first;
		int c=0;
		while ((p != last)&&(p!=NULL)) {
            c++;
			p=p->next;
        }
		return c;
	}
};

class lex_list { //класс FIFO список лексем
    struct fl {
        type_of_lex lex;
        fl * next;
    };
    fl * first;
    fl * last;

    public:
        lex_list() {
            first = new fl;
            last = first;
            first->next=NULL;
        }

        ~lex_list() { //деструктор, освобождающий всю память в случае чего
            fl * p;
            while ((first != last)&&(first!=NULL)) {
                p = first;
                first = first -> next;
                delete p;
            }
            delete first;
        }

    void add_fl(type_of_lex f) { //метод, добавляющий лексему в списко
        last -> lex = f;
        last -> next = new fl;
        last = last -> next;
	last->next=NULL;
    }

    int get_fl(type_of_lex & a) { //получение лексемы из списка (значение записывается в параметр)
        fl * p;
        if ((first == last)||(first==NULL))
            return 0;
        p = first;
        a = p -> lex;
        first = first -> next;
        delete p;
        return 1;
    }
};

#endif
