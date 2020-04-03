#if !defined TREE_CPP
#define TREE_CPP

#include <iostream>
#include "file.h"
#include "tree.h"
//#include "file.cpp"

using namespace std;

	void error(const string s, int fatal = 0){
		cout << s << endl;
		if (fatal == 1){
			cout << "fatal error" << endl;
		}
	};

	File &Node::GetData(){
		return *data;
	}

	void Node::PrintSubTree() const{
		if (left != NULL)
			left->PrintSubTree();
		cout << *data << endl;
		if (right != NULL)
			right->PrintSubTree();
	}

	void Node::DeleteSubTree(){
		if (left != NULL)
			left->DeleteSubTree();
		delete left;
		left = NULL;
		if (right != NULL)
			right->DeleteSubTree();
		delete right;
		right = NULL;
		delete data;
	}

	bool Node::AddToSubTree(File *_data){
		if (*data == *_data){
			error("This word already exist");
			return false;
		}
		if (*data > *_data){
			if (left == NULL)
				left = new Node(_data);
			else
				return left->AddToSubTree(_data);
		}
		if (*data < *_data){
			if (right == NULL)
				right = new Node(_data);
			else
				return right->AddToSubTree(_data);
		}
		return true;
	}


std::ostream& operator<< (std::ostream &out, const Node & node){
	out << node.data;
	return out;
};

std::ostream& operator<< (std::ostream &out, const Node * node){
	out << node->data;
	return out;
};

#endif
