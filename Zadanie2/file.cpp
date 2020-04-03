#if !defined FILE_CPP
#define FILE_CPP

#include <iostream>
#include "file.h"

	File::File (const string _name){
		name = _name;
		key = new int (rand());
		quantity++;
	}
	
	File::File (const File & f){
		name = f.name;
		key = new int (rand());
		quantity++;
	}
	
	bool File::operator==(const File &f){
		return name == f.name;
	}
	bool File::operator!=(const File &f){
		return name != f.name;
	}
	bool File::operator>(const File &f){
		return name > f.name;
	}
	bool File::operator<(const File &f){
		return name < f.name;
	}

unsigned int File::quantity = 0;



std::ostream& operator<< (std::ostream &out, const File & f){
	out << "Name: " << f.name << endl << "Key: " << *(f.key);
	return out;
};



std::istream& operator>> (std::istream &in, File & f){
	in >> f.name;
	return in;
};

#endif
