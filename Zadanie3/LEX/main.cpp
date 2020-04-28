#include <iostream>

#include <fstream>

#include "lex.h"

#include "ident.h"

#include "lists.h"

#include "scan.h"

using namespace std;



int main(int argc, char ** argv) {
    if (argc<3) return 0;
    res.open(argv[2]);
    type_of_lex t = lex_NULL;
    lex f;
    Scanner ob1(argv[1]);
    while (t != lex_FIN) {
        f = ob1.get_lex();
        t = f.get_type();
        record(t);
    }
    res << endl << endl << "DIFFERENT IDENTIFIERS ARE: "; //вывод различных идентификаторов
    for (int i = 0; i < TID.get_top(); ++i)
        res << TID[i].get_name() << ' ';
    res << endl << endl << "IDENTIFIERS IN PROGRAM SEQUENCE: "; //вывод идентификаторов в порядке появления в программе
    IL.print_ids();
    res << endl << endl << "DIGITS IN PROGRAM SEQUENCE: "; //вывод чисел в порядке появления в программе
    DL.print_digits();
    res << endl << endl << "STRINGS IN PROGRAM SEQUENCE:\n"; //вывод строк в порядке появления в программе
    SL.print_strs();
    res.close();
    return 0;
}
