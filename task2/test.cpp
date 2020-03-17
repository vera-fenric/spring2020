#include <iostream>
#include <string>
#include <cstring>
using namespace std;
class file
{
	string name;
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

class txt_file: public file
{
	string * summary;
	unsigned int size;
	
	public:
	txt_file (const string str1, const string str2): file(str1, str2)
	{
		
	}
	const string get_type()
	{
		return "txt file";    	
	}
};

class video_file: public file
{
	unsigned int duration;
	unsigned int fps;
	unsigned int height;
	unsigned int width;
	
	public:
	video_file (const string str1, const string str2): file(str1, str2)
	{
		
	}
	const string get_type()
	{
		return "video file";    	
	}
	
	void change_duration(int d){
		if (d<=0 || d > duration) throw ("Wrong params on func change_duration");
		duration=d;
	}
};

class image_file: public file
{
	unsigned int height;
	unsigned int width;
	
	public:
	image_file (const string str1, const string str2): file(str1, str2)
	{
		
	}
	const string get_type()
	{
		return "image file";    	
	}
	
	void crop (int h, int w){
		if (h<=0 || h > height || w <=0 || w > weight) throw ("Wrong params on func crop");
		height=h;
		width=w;
	}
	
};



unsigned int file::quantity = 0;	




int main()
{


return 0;
}
