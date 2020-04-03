#if !defined FILE_H
#define FILE_H

#include <iostream>
using namespace std;
class File{
	string name;
	int * key;
	static unsigned int quantity;
	
  public:
	virtual const string get_type() = 0;
	friend ostream& operator<< (ostream &out, const File & f);
	friend istream& operator>> (istream &in, File & f);
	
	File (const string _name);
	
	File (const File & f);
		
	virtual File & operator= (const File & f){
		if (this == &f)	
			return *this;
		name = f.name;			
		delete key;
		key = new int (rand());
		return *this;
	}
	
	virtual File & operator() (string & _name){
		name = _name;
		return *this;
	}
	
	static unsigned int get_quantity(){
		return quantity;
	}
	
	virtual ~File(){
		delete key;
		quantity--;
	}
	
	
	bool operator==(const File &f);
	bool operator!=(const File &f);
	bool operator>(const File &f);
	bool operator<(const File &f);
};

class Txt_File: public File{
	string * summary;
	unsigned int size;
	
	public:
	Txt_File (const string str): File(str){
		
	}
	const string get_type() override{
		return "txt file";    	
	}
};

class Video_File: public File{
	unsigned int duration;
	unsigned int fps;
	unsigned int height;
	unsigned int width;
	
	public:
	Video_File (const string str): File(str){
		
	}
	const string get_type() override{
		return "video file";    	
	}
};

class Image_File: public File{
	unsigned int height;
	unsigned int width;
	
	public:
	Image_File (const string str): File(str){
		
	}
	const string get_type() override{
		return "image file";    	
	}
	
};

#endif
