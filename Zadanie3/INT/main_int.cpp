#include "execute.h"
#include <iostream>
using namespace std;

int main (int argc, char* argv[]) {
  try {
	Interpretator I ("test.txt");
	I.interpretation ();
  }
  catch (char c) {
    cout << "unexpected symbol " << c << endl;
    return 1;
  }
  catch (Lex l) {
    cout << "unexpected lexeme" << l << endl;
    return 1;
  }
  catch ( const char *source ) {
    cout << source << endl;
    return 1;
  }
}