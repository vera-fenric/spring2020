#ifndef SCANNER
#define SCANNER

#include "lex.h"

#include "ident.h"

#include "lists.h"

#define BUFSIZE 200

digit_list DL, IL; //список чисел DL и номеров в таблице идентификаторов IL
str_list SL; //список строк
lex_list FL; //список лексем

class Scanner { //класс сканнер, вся работа лексического анализатора в нем
    enum state {
        H,
        IDENT,
        NUMB,
        STR,
        COM,
        ALE,
        DELIM,
        NEQ //перечислимый тип с именами состояний ДС
    };
    static
    const char * TW[]; //массив из служебных слов
    static type_of_lex words[]; //масив лексем, отвечающих данным служебным словам
    static
    const char * TD[]; //массив из разделителей
    static type_of_lex dlms[]; //массив лексем, отвечающих данным разделителям
    state CS; //состояние
    FILE * fp;
    char c;
    char buf[BUFSIZE]; //буффер для сохранение слов из программы
    int buf_top; //текущий размер буффера

    void clear() { //отчистка буффера
        buf_top = 0;
        for (int j = 0; j < BUFSIZE; ++j)
            buf[j] = '\0';
    }

    void add() { //добавление символа в буффер
        if (buf_top < BUFSIZE)
            buf[buf_top++] = c;
    }

    int look(const char * s,
        const char ** list) { //поиск совпадений строки s со строкой из массива строк list
        int i = 0;
        while (list[i]) {
            if (!strcmp(s, list[i]))
                return i;
            i++;
        }
        return 0;
    }

    void gc() { //получение символа из файла и запись в c
        c = fgetc(fp);
    }

    public:
        explicit Scanner(const char * program) { //конструктор
			fp = NULL;
            fp = fopen(program, "r"); //открытие файла с программой
			if (fp == NULL) throw "FILE CANNOT BE OPENED";
            CS = H; //начальное состояние
            clear(); //отчистка буффера
            gc(); //получение первого символа
        }
        ~Scanner() { //деструктор
            fclose(fp);
        }

    Lex get_lex(); //получение лексемы из файла с программой (ЛА)
};

const char * Scanner::TW[] = //инициализация массив служебных слов
    {
        "", //0
        "and", //1
        "or", //2
        "not", //3
        "program", //4
        "if", //5
        "else", //6
        "do", //7
        "while", //8
        "read", //9
        "write", //10
        "string", //11
        "bool", //12
        "int", //13
        "true", //14
        "false", //15
        "break", //16
        NULL
    };

const char * Scanner::TD[] //инициализация массива из разделителей
{
    "", //0
    ";", //1
    "!=", //2
    ",", //3
    "=", //4
    "(", //5
    ")", //6
    "==", //7
    "<", //8
    ">", //9
    "<=", //10
    ">=", //11
    "+", //12
    "-", //13
    "*", //14
    "/", //15
    "{", //16
    "}", //17
    NULL
};

type_of_lex Scanner::words[] = //инициализация масива лексем, отвечающих служебным словам
    {
        lex_NULL,
        lex_AND,
        lex_OR,
        lex_NOT,
        lex_PROGRAM,
        lex_IF,
        lex_ELSE,
        lex_DO,
        lex_WHILE,
        lex_READ,
        lex_WRITE,
        lex_STRING,
        lex_BOOL,
        lex_INT,
        lex_TRUE,
        lex_FALSE,
        lex_BREAK,
        lex_NULL
    };

type_of_lex Scanner::dlms[] = //инициализация масива лексем, отвечающих разделителям
    {
        lex_NULL,
        lex_SEMICOLON,
        lex_NEQ,
        lex_COMMA,
        lex_ASSIGN,
        lex_LPAREN,
        lex_RPAREN,
        lex_EQ,
        lex_LSS,
        lex_GTR,
        lex_LEQ,
        lex_REQ,
        lex_PLUS,
        lex_MINUS,
        lex_TIMES,
        lex_SLASH,
        lex_LBRAC,
        lex_RBRAC,
        lex_NULL
    };

Lex Scanner::get_lex() {
    int d = 0, j=0;
    CS = H;
    try {
        do {
	    //cout << "bla" << endl;
            switch (CS) {
                case H:
                    if (c == '\n')
                        res << '\n';
                    if ((c == ' ') || (c == '\n') || (c == '\r') || (c == '\t')) //пропуск ненужных символов
                        gc();
                    else if (isalpha(c)) { //если буква, подготовка к состоянию IDENT (идентификатор)
                        clear();
                        add();
                        gc();
                        CS = IDENT;
                    } else if (isdigit(c)) { //если число, подготовка к состоянию NUMB (число)
                        d = c - '0';
                        gc();
                        CS = NUMB;
                    } else if (c == '/') { //если /, подготовка к состоянию COM (комментарий)
                        clear();
                        add();
                        gc();
                        CS = COM;
                    } else if (c == '"') { //если ", подготовка к состоянию STR (строка)
                        clear();
                        gc();
                        CS = STR;
		    } else if ((c == '=') || (c == '<') ||
                        (c == '>')) { //если один из символов, к которым может добавиться =
                        clear(); //подготовка к состоянию ALE (?)
                        add();
                        gc();
                        CS = ALE;
                    } else if (c == EOF) { //если конец файла, возращаем лексему конца
                        FL.add_fl(lex_FIN); //добавка в список лексем
                        return Lex(lex_FIN);
                    } else if (c == '!') { //если !, подготовка к состоянию NEQ (не равно)
                        clear();
                        add();
                        gc();
                        CS = NEQ;
                    } else if (c == '#') {
			//cout << "coment" << endl;
			while ((c!='\n')&&(c!=EOF)){
				gc();
			}
		        clear();
		        gc();
			//cout << "com" << endl;
                        break;			
		    } else
                        CS = DELIM; //в остальных случаях переход к состоянию DELIM
                    break;
                case IDENT:
                    if ((isalpha(c)) || (isdigit(c)) || c=='_') { //в этом состоянии считываются символы пока это буква или число
                        add();
                        gc();
                    } else if ((j = look(buf, TW))!=0) { //затем проверка на наличие такого служебного слова
                        FL.add_fl(words[j]); //добавка в список лексем
                        return Lex(words[j], j); //возвращаем полученное служебное слово в виде лексемы
                    } else {
                        j = TID.put(buf); //иначе добавляем идентификатор в таблицу
                        IL.add_dig(j); //добавка в список идентификаторов
                        FL.add_fl(lex_ID); //добавка в список лексем
                        return Lex(lex_ID, j); //возвращаем лексему идентификатор
                    }
                    break;
                case NUMB:
                    if (isdigit(c)) { //в этом состоянии считываются символы пока это число
                        d = d * 10 + (c - '0');
                        gc();
                    } else {
                        DL.add_dig(d); //добавка в список чисел
                        FL.add_fl(lex_NUMB); //добавка в список лексем
                        return Lex(lex_NUMB, j); //возвращение лексемы число
                    }
                    break;
                case STR: //состояние STR
                    if (c == '"') { //встретился маркер конца строки
                        CS = H;
                        gc();
                        SL.add_str(buf); //добавка в список строк
                        FL.add_fl(lex_STR); //добавка в список лексем
                        return Lex(lex_STR); //возвращение лексемы строка
                    } else {
                        if (c == '\\') //если при считывании строки встретился \ то считывается доп символ
                            gc(); //нужно для добавки в строку символа "
                        add();
                        gc();
                    }
                    break;
                case COM: //состояние комментарий
                    if (c == '*') { //если звездочка, то получен маркер начала комментария (/*)
                        gc();
                        while (true) {
                            if (c == '*') { //считывается посл-ть символов пока не встретиться маркер конца комментария (*/)
                                gc();
                                if (c == '/')
                                    break;
                            }
                            gc();
                        }
                    } else { //в ином случае мы получили лексему /
                        j = look(buf, TD);
                        FL.add_fl(lex_SLASH);
                        return Lex(lex_SLASH, j);
                        break;
                    }
                    CS = H;
                    gc();
                    clear();
                    break;
                case ALE: //состояние ALE
                    if (c == '=') { //если встретилось равно, то один из разделителей <=, >=, ==
                        add();
                        gc();
                        j = look(buf, TD);
                        FL.add_fl(dlms[j]);
                        return Lex(dlms[j], j);
                    } else {
                        j = look(buf, TD); //иначе <, > или =
                        FL.add_fl(dlms[j]);
                        return Lex(dlms[j], j);
                    }
                    break;
                case NEQ: //состояние NEQ
                    if (c == '=') { //если равно, то разделитель !=
                        add();
                        gc();
                        j = look(buf, TD);
                        FL.add_fl(lex_NEQ);
                        return Lex(lex_NEQ, j);
                    } else //иначе неправильное использование !, выдаем ошибку
                        throw '!';
                    break;
                case DELIM: //состояние DELIM
                    clear();
                    add();
                    if ((j = look(buf, TD))!=0) { //обработка оставшихся символов
                        gc();
                        FL.add_fl(dlms[j]);
                        return Lex(dlms[j], j);
                    } else //если полученное из файла символ не совпадает с оставшимися разделителями, то ошибка
                        throw c;
                    break;
            }
        } while (true);
    } catch (char ch) { //небольшая обработка ошибок
        if (ch == '!')
            res << "------Wrong use of '!' symbol------ ";
        else
            res << "------Wrong use of '" << ch << "'------ ";
        gc();
    }
    return Lex();
}

#endif
