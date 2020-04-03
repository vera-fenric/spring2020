#if !defined TREE_H
#define TREE_H


#include <iostream>
#include "file.h"

using namespace std;

void error(const string s, int fatal);

class Node
{
	File *data;
	Node *left;
	Node *right;

  public:
	Node(File * _data) : data(_data), left(NULL), right(NULL){};

	friend std::ostream& operator<< (std::ostream &out, const Node &);
	friend std::ostream& operator<< (std::ostream &out, const Node *);

	File &GetData();
	void PrintSubTree() const;	
	void DeleteSubTree();
	bool AddToSubTree(File *_data);
};

#endif
