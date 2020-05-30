#ifndef CLASSIDENT
#define CLASSIDENT

#include "lex.h"

#include <cstring>


class Ident { //класс идентификатор
	protected:
    char * name; //имя
    bool declare;
    type_of_lex type; //тип идентификатора по типу лексемы
    bool assign;
	int value;

    public:
    Ident() {
        declare = false;
        assign = false;
        name = NULL;
		type = lex_NULL;
    }
	~Ident() {
        if (name!=NULL)
			delete[] name;
	}

    char * get_name() {
        return name;
    }

    void set_name(const char * n) {
		if (name!=NULL)
			delete[] name;
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }

    bool get_declare() const {
        return declare;
    }

    void set_declare() {
        declare = true;
    }

    type_of_lex get_type() const {
        return type;
    }

    void set_type(type_of_lex t) {
        type = t;
    }

    bool get_assign() const {
        return assign;
    }

    void set_assign() {
        assign = true;
    }
	
	int get_value() const {
		return value;
	}
	
	void set_value (int v){
		value = v;
	}
};


class tabl_ident { //класс таблица идентификаторов
    Ident * p; //указатель на идентификаторы
    int size; //максимальный размер
    int top; //вершина таблицы

    public:
        explicit tabl_ident(int max_size) {
            p = new Ident[size = max_size];
            top = 0;
        }

        ~tabl_ident() {
            delete[] p;
        }

    int get_top() const {
        return top;
    }

    Ident & operator[](int k) { //перегрузка операции [] для получения непосредственно идентификатора
		if ((k<0)||(k>=top))
			throw "Wrond range";
        return p[k];
    }

    int put(const char * buf) { //метод ищущий идентификатор в таблице
        for (int j = 0; j < top; ++j) //если такого идентификатора еще не было, то он добавляется
            if (!strcmp(buf, p[j].get_name()))
                return j;
        p[top].set_name(buf);
        ++top;
        return top - 1;
    }
};


#define MAXLEN 200

tabl_ident TID(MAXLEN); //непосредственно таблица

#endif
