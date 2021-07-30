#include "sequoia.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;

const string INPUT_FILE("input.txt");
const string OUTPUT_FILE("output.txt");

//Checks whether the height of this node and all of its
// descendents are calculated correctly
//Returns the height of this node if correct
//Throws a runtime_error if incorrect
int SequoiaNode::checkHeight() const
{
	int lht = 0, rht = 0;
	if (left != nullptr)
		lht = left->checkHeight();
	if (right != nullptr)
		rht = right->checkHeight();
	if (lht > rht && height != lht + 1)
	{
		cout << "Height of node " << value << " incorrect.\n"
			<< "Left height = " << lht << '\n'
			<< "Right height = " << rht << '\n'
			<< "Node height = " << height << endl;
		throw runtime_error("Bad height");
	}
	return height;
}

//Returns true if the height is calculated correctly
// in every node in this tree
bool Sequoia::checkHeight() const
{
	try
	{
		root->checkHeight();
		return true;
	}
	catch (runtime_error& ex)
	{
		return false;
	}
}

//Returns whether the tree rooted at this SequoiaNode is tall
//Assumes height is calculated correctly
bool SequoiaNode::isTall() const
{
	int lht = 0, rht = 0;
	if (left != nullptr)
	{
		lht = left->height;
		if (!left->isTall())
			return false;
	}
	if (right != nullptr)
	{
		rht = right->height;
		if (!right->isTall())
			return false;
	}
	if (lht >= 2 * rht || rht >= 2 * lht)
		return true;
	else
	{
		cout << "Node " << value << " is not tall:\n"
			<< "Left height = " << lht << '\n'
			<< "Right height = " << rht << '\n';
		return false;
	}
}

//Returns whether a Sequoia is tall
//Assumes height is calculated correctly
bool Sequoia::isTall() const
{
	if (root == nullptr)
		return true;
	else
		return root->isTall();
}

//Print Sequoia to given ostream
ostream& operator<<(ostream& out, const Sequoia& tree)
{
	if (tree.root == nullptr)
		return out << "empty";
	else
		return out << tree.root;
}

//Print the tree rooted at this SequoiaNode to out
ostream& operator<<(ostream& out, const SequoiaNode* node)
{
	out << '(' << node->value;
	if (node->left != nullptr)
		out << " L:" << node->left;
	if (node->right != nullptr)
		out << " R:" << node->right;
	return out << ')';
}

void SequoiaNode::fixHeightInsert()
{
	
	this->updateHeight();
	
	if (this->right == nullptr || this->left == nullptr) {
		if (this->parent != nullptr) {
			this->parent->fixHeightInsert();
		}
	}
	else if ((2 * this->right->height > this->left->height) || (2 * this->left->height > this->right->height)){
		if (2 * right->height > this->left->height && this->right->height <= this->left->height) {
			this->right->rotateLeft();
		}
		else if (2 * this->left->height > this->right->height && this->left->height <= this->right->height) {
			this->left->rotateRight();
		}

		this->updateHeight();
		if (this->parent != nullptr) {
			this->parent->fixHeightInsert();
		}
	}
}

void SequoiaNode::fixHeightRemove()
{
	this->updateHeight();
	if (this->right == nullptr || this->left == nullptr) {
		if (this->parent != nullptr) {
			this->parent->fixHeightRemove();
		}
	}
	else if ((2 * this->right->height > this->left->height) || (2 * this->left->height > this->right->height)){
		if (this->right->height <= this->left->height && 2 * this->right->height > this->left->height) {
			this->right->rotateLeft();
		}
		else if (this->right->height >= this->left->height && this->right->height < 2 * this->left->height) {
			this->left->rotateRight();
		}
		updateHeight();
		if (this->parent != nullptr) {
			this->parent->fixHeightRemove();
		}
	}
}


void Sequoia::remove(int x)
{
	if (root == nullptr)
		return; 
	// search for given value
	SequoiaNode* removedNode = root->search(x);
	// if value not exist
	if (removedNode == nullptr)
		return;
	else
	{
		if (removedNode == root)
		if (root->left != nullptr)
			root = root->left;
		else
			root = root->right;
		removedNode = removedNode->remove();
		delete removedNode;
	}
}

SequoiaNode* SequoiaNode::search(int x)
{
	// if the given value is the value of called Node
	if (x == this->value)
		return this;
	// if the given value less than the value of called Node
	// and left child not equal to nullptr then go to search in left sub tree
	if (x < this->value && this->left != nullptr)
		return this->left->search(x);
	// if the given value greater than the value of called Node
	// and right child not equal to nullptr then go to search in right sub tree
	if (x > value && this->right != nullptr)
		return this->right->search(x);
	// not match any case return nullptr
	return nullptr;
}


//Function to remove this node from its Sequoia tree
SequoiaNode* SequoiaNode::remove()
{
	// removed node has no child
	if (this->right == nullptr && this->left == nullptr)
	{
		if (this->parent != nullptr)
		if (this->parent->left == this)
			this->parent->left = nullptr;
		else
			this->parent->right = nullptr;

		this->parent->fixHeightRemove();
	}
	// removed node has one child
	else if (this->right == nullptr && this->left != nullptr)
	{
		this->left->parent = this->parent;
		if (this->parent->left == this)
			this->parent->left = this->left;
		else
			this->parent->right = this->left;
		this->parent->fixHeightRemove();
	}
	// removed node has one child
	else if (this->right != nullptr && this->left == nullptr)
	{
		this->right->parent = this->parent;
		if (this->parent->left == this)
			this->parent->left = this->right;
		else
			this->parent->right = this->right;
		this->parent->fixHeightRemove();
	}
	// removed node has two child
	else
	{
		SequoiaNode* tempNode = this->left;
		int tempValue = this->value;
		while (tempNode->right != nullptr)
			tempNode = tempNode->right;
		this->value = tempNode->value;
		tempNode->value = tempValue;
		return tempNode->remove();
	}
	this->left = this->right = nullptr;
	return this;

}

void Sequoia::insert(int x)
{
	// if tree is Empty then add the new node as the root Node
	if (size == 0)
		this->root = new SequoiaNode(x);
	// if the tree has many Nodes 
	else
		this->root->insert(x);
	// increment size
	size++;
	
	while (this->root->parent != nullptr)
		this->root = this->root->parent;
}

void SequoiaNode::insert(int x)
{
	// if new value less than the value of called node
	// then add it to the left sub tree
	if (x < this->value)
	{
		// if left not null then go to it's left
		if (this->left != nullptr)
			this->left->insert(x);
		else
		{
			// left is nullptr then add the new value as left child
			this->left = new SequoiaNode(x);
			// set the parent of new node to the called node
			((SequoiaNode*)this->left)->parent = this;
			// fix height
			fixHeightInsert();
		}
	}
	// if new value greater than the value of called node
	// then add it to the right sub tree
	else
	{
		if (this->right != nullptr)
			this->right->insert(x);
		else
		{
			this->right = new SequoiaNode(x);
			((SequoiaNode*)this->right)->parent = this;
			// fix height
			fixHeightInsert();
		}
	}
}

void SequoiaNode::updateHeight()
{
	if (this->left == nullptr && this->right == nullptr){
		this->height = 1;
		return;
	}
	if (this->left != nullptr && this->right == nullptr) {
		this->height = this->left->height + 1;
		return;
	}
	if (this->left == nullptr && this->right != nullptr) {
		this->height = this->right->height + 1;
		return;
	}

	if (this->left->height > this->right->height) {
		this->height = this->left->height + 1;
	}
	else {
		this->height = this->right->height + 1;
	}
}

void SequoiaNode::rotateLeft() {
	SequoiaNode* tempNode = this->parent;
	this->parent = this->parent->parent;
	if (this->parent != nullptr)
	{
		if (this->parent->left == tempNode)
			this->parent->left = this;
		else
			this->parent->right = this;
	}
	tempNode->parent = this;
	tempNode->right = this->left;
	this->left = tempNode;
	if (tempNode->right != nullptr) {
		tempNode->right->parent = tempNode;
	}
	updateHeight();
}

void SequoiaNode::rotateRight() {
	SequoiaNode* tempNode = parent;
	parent = parent->parent;
	if (parent != nullptr)
	{
		if (parent->left == tempNode)
			parent->left = this;
		else
			parent->right = this;
	}
	tempNode->parent = this;
	tempNode->left = right;
	right = tempNode;
	if (tempNode->left != nullptr) {
		tempNode->left->parent = tempNode;
	}
	updateHeight();
}

int main()
{
	vector<int> ins, del;
	ifstream in(INPUT_FILE);
	ofstream out(OUTPUT_FILE);
	string line;
	istringstream iss;
	int temp;
	Sequoia seq;

	if (!in.is_open())
	{
		cout << "Error opening " << INPUT_FILE << endl;
		return 1;
	}
	if (!out.is_open())
	{
		cout << "Error opening " << OUTPUT_FILE << endl;
		return 1;
	}

	//Read first line of input.txt into ins
	getline(in, line);
	iss.str(line);
	while (iss >> temp)
		ins.push_back(temp);
	iss.clear();

	//Read second line of input.txt into del
	getline(in, line);
	iss.str(line);
	while (iss >> temp)
		del.push_back(temp);
	in.close();

	//Insert all values of ins into seq
	//Check for problems in tree
	for (int i : ins)
	{
		seq.insert(i);
		if (!seq.checkHeight() || !seq.isTall())
		{
			cout << "Sequoia invalid after adding " << i
				<< ":  " << seq << endl;
			return 1;
		}
	}
	//cout << seq << endl;
	out << seq << endl;

	//Remove all values in del from seq
	//Check for problems in tree
	//Assume everything in del is in tree
	for (int x : del)
	{
		seq.remove(x);
		if (!seq.checkHeight() || !seq.isTall())
		{
			cout << "Sequoia invalid after removing " << x
				<< ":  " << seq << endl;
			return 1;
		}
	}
	//cout << seq << endl;
	out << seq << endl;
	out.close();

	return 0;
}