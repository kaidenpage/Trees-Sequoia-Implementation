#ifndef __SEQUOIA_H
#define __SEQUOIA_H

#include <iostream>
using namespace std;

class SequoiaNode;
class Sequoia;

class SequoiaNode
{
	friend Sequoia;
	friend ostream& operator<<(ostream&, const SequoiaNode*);
private:
	int value;
	SequoiaNode* left;
	SequoiaNode* right;
	SequoiaNode* parent;
	int height;
public:
	//Basic constructor and destructor
	SequoiaNode(int x) { value = x; height = 1; left = right = parent = nullptr; }
	~SequoiaNode() { delete left; delete right; }
	//Optional:  copy constructor or copy assignment

	//These functions are *required*
	void insert(int x);
	SequoiaNode* remove();
	int checkHeight() const;
	bool isTall() const;

	//May add any number of additional functions
	//E.g.,
	SequoiaNode* search(int x);
	void fixHeightInsert();
	void fixHeightRemove();
	void rotateLeft();
	void rotateRight();
	void updateHeight();
	bool isLeftChild() const
	{
		return parent != nullptr && parent->left == this;
	}

	//Returns whether this node is a right child
	//Returns false for root
	bool isRightChild() const
	{
		return parent != nullptr && parent->right == this;
	}

	SequoiaNode* findMax()
	{
		if (right == nullptr)
			return this;
		else
			return right->findMax();
	}

};

class Sequoia
{
	friend ostream& operator<<(ostream&, const Sequoia&);
private:
	SequoiaNode* root;
	int size;
public:
	//Basic constructor and destructor
	Sequoia() { root = nullptr; size = 0; }
	~Sequoia() { delete root; }
	//Optional: copy constructor or copy assignment

	//These functions are required
	void insert(int x);
	void remove(int x);
	bool checkHeight() const;
	bool isTall() const;

	//May add any number of additional functions
};

//Print functions (required)
ostream& operator<<(ostream&, const Sequoia&);
ostream& operator<<(ostream&, const SequoiaNode*);

#endif //__SEQUOIA_H