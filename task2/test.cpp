#include <iostream>
#include <string>
#include <cstring>
using namespace std;
class file
{
	string name;
	unsigned int size;
	string * path;
	static unsigned int quantity;
	public:
	virtual const string get_type() = 0;
	file (const string _name = "Filename", const string _path = "~/")
	{
		name = _name;
		path = new string (_path);
		quantity++;
	}
	file (const file & f)
	{
		name = f.name;
		path = new string (*f.path);
		quantity++;
	}
	file & operator= (const file & f)
	{
		if (this == &f)	
			return *this;
		name = f.name;	
		delete path;			
		path = new string (*f.path);
		quantity++;
		return *this;
	}
	static unsigned int get_quantity()
	{
		return quantity;
	}
	~file()
	{
		delete path;	
		quantity--;
	}



};

class txtfile: public file
{
	public:
	txtfile (const string str1, const string str2): file(str1, str2)
	{
		
	}
	const string get_type()
	{
		return "txt file";    	
	}
};


unsigned int file::quantity = 0;	




int main()
{


return 0;
}
