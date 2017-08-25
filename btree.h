#pragma once
#pragma once

#include <iostream>
#include "node.h"
#include <iostream>
#include <algorithm>

using namespace std;

template<class Type>
class BTree {
private:
	int order;
	int size;
	int height;
	int p_followed;
	int p_add;
	int p_find;
	static Node<Type>* root;
	Node<Type>* tree;
public:
	BTree(int inOrder = 3);

	~BTree();

	void insert(Type* key, Node<Type>* subtree = root, bool splitting = false);

	Node<Type>* traverse(Type* key, Node<Type>* subtree = root);

	void split(Node<Type>* subtree);

	//Type* remove(Type* key);

	Type* find(Type* key);

	int getSize();

	int getHeight(Node<Type>* subtree = root);

	int getPointersFollowed();

	int getPointersAdd();

	int getPointersFind();

	void emptyTree(Node<Type>* subtree = root);

	void printTree(Node<Type>* subtree = root);
};

template<class Type>
Node<Type>* BTree<Type>::root = nullptr;

template<class Type>
BTree<Type>::BTree(int inOrder) {
	order = inOrder;
	height = 0;
	size = 0;
	p_followed = 0;
	p_add = 0;
	p_find = 0;
	tree = nullptr;
	root = tree;
}

template<class Type>
void BTree<Type>::insert(Type* key, Node<Type>* subtree, bool splitting) {
	Node<Type>* target = subtree;
	p_add++;
	if (subtree == root && root != nullptr && !root->leaf && !splitting) {
		target = traverse(key, subtree);
		p_add += p_followed;
		p_followed = 0;
	}
	if (tree == nullptr) {
		tree = new Node<Type>(key, order);
		tree->root = true;
		tree->leaf = true;
		p_add += 2;
		root = tree;
		size++;
		return;
	}
	target->insert(key);
	size++;
	p_add++;
	if (target->length == order) {
		split(target);
	}
}

template<class Type>
Type* BTree<Type>::find(Type* key) {
	Node<Type>* target_node = traverse(key);
	p_find += p_followed;
	p_followed = 0;
	return target_node->find(key);
}

template<class Type>
Node<Type>* BTree<Type>::traverse(Type* key, Node<Type>* subtree) {
	if (subtree != nullptr) {
		int i = 0;
		p_followed += 3;
		while (subtree->keys[i] && *subtree->keys[i] < *key && i < subtree->length) {
			i++;
		}
		p_followed++;
		if (subtree->children[i] == nullptr) {
			return subtree;
		}
		p_followed++;
		traverse(key, subtree->children[i]);
	}
	else {
		return nullptr;
	}
}

template<class Type>
void BTree<Type>::split(Node<Type>* subtree) {
	int i = 0; //Position of value to send to parent
	int l = 0; //Num of vals to left
	p_add++;
	int r = subtree->order - 1; //Num of vals to right
								//Find first val where diff # vals on l/r is > 1
	while (abs(l - r) > 1) { //If diff > 1, break
		i++;
		l++;
		r--;
	}
	p_add++;
	Type* key = subtree->keys[i]; //subtree->remove(subtree->keys[i]);
								  //--------Set new right branch--------------//
	p_add += 9;
	Node<Type>* right = new Node<Type>(subtree->keys[i + 1], order);
	right->leaf = true;
	right->parent = subtree->parent;
	right->children[0] = subtree->children[i + 1];
	if (right->children[0] != nullptr) {
		right->children[0]->parent = right;
		p_add++;
	}
	subtree->children[i + 1] = nullptr;
	subtree->leaf = true;
	//-------Iterate through keys and assign appropriate keys to right, removing from left----//
	p_add++;
	for (int k = i + 2; k < subtree->order; k++) {
		p_add++;
		insert(subtree->keys[k], right);
	}
	for (int k = i; k < order; k++) {
		p_add++;
		subtree->remove(subtree->keys[k]);
	}
	//-------Iterate through children, removing from left, adding to right----//
	for (int k = i + 2; k < order + 1; k++) {
		p_add += 3;
		right->children[k - i - 1] = subtree->children[k];
		if (right->children[k - i - 1] != nullptr) {
			right->children[k - i - 1]->parent = right;
			p_add += 2;
		}
		p_add++;
		subtree->children[k] = nullptr;
	}
	//------Check for root split---------------//
	p_add++;
	if (subtree->parent == nullptr) { //Split root case
		p_add += 6;
		subtree->parent = new Node<Type>(key, order);
		tree = subtree->parent;
		root = tree;
		root->root = true;
		right->parent = root;
		subtree->parent = root;
		subtree->root = false;
	}
	//------Not root, insert into parent------//
	else {
		p_add++;
		subtree->parent->insert(key);
	}
	//-----Get and set position of new left/right branches
	p_add += 5;
	i = 0;
	while (subtree->parent != nullptr && subtree->parent->keys[i] && *subtree->parent->keys[i] < *key && i < subtree->parent->length) {
		
		i++;
	}
	p_add += 4;
	subtree->parent->children[i] = subtree;
	subtree->parent->children[i + 1] = right;
	for (int i = 0; i < order; i++) {
		p_add++;
		if (subtree->children[i] != nullptr) {
			p_add++;
			subtree->leaf = false;
		}
		p_add++;
		if (right->children[i] != nullptr) {
			p_add++;
			right->leaf = false;
		}
	}
	//----Since parent may have increased size, check for split--//
	p_add += 2;
	if (subtree->parent->length == order) {
		p_add++;
		split(subtree->parent);
	}
}

template<class Type>
int BTree<Type>::getSize() {
	return size;
}

template<class Type>
int BTree<Type>::getHeight(Node<Type>* subtree) {
	if (subtree == nullptr) { //Empty tree has height 0
		return 0;
	}
	for (int i = 0; i < order - 1; i++) {
		return 1 + max(getHeight(subtree->children[i]), getHeight(subtree->children[i + 1])); //Return the max of the left and right heights recursively
	}
}

template<class Type>
int BTree<Type>::getPointersFollowed() {
	return p_add + p_find;
}

template<class Type>
int BTree<Type>::getPointersAdd() {
	return p_add;
}

template<class Type>
int BTree<Type>::getPointersFind() {
	return p_find;
}

template<class Type>
void BTree<Type>::printTree(Node<Type>* subtree) { //Level-order Print function
	if (subtree != nullptr) {
		cout << "---------------------------------------------" << endl;
		for (int v = 0; v < 4; v++) {
			switch (v) {
			case 0: {
				if (subtree->root) {
					cout << "| Root ";
				}
				else if (subtree->leaf) {
					cout << "| Leaf ";
				}
				else {
					cout << "| Intermediate ";
				}
				break;
			}
			case 1: {
				if (subtree->parent != nullptr) {
					cout << "| Parent's Keys: " << *subtree->parent->keys[0];
					for (int i = 1; i < subtree->parent->length; i++) {
						cout << ", " << *subtree->parent->keys[i];
					}
					cout << " ";
				}
				else {
					cout << "| Parent: NULLPTR ";
				}
				break;
			}
			case 2: {
				cout << "| Current's Keys: " << *subtree->keys[0];
				for (int i = 1; i < subtree->length; i++) {
					cout << ", " << *subtree->keys[i];
				}
				cout << " |" << endl;
				break;
			}
			case 3: {
				for (int c = 0; c < order + 1; c++) {
					if (subtree->children[c] != nullptr) {
						Node<Type>* child = subtree->children[c];
						cout << "| Child[" << c << "] Keys: " << *child->keys[0];
						for (int i = 1; i < child->length; i++) {
							cout << ", " << *child->keys[i];
						}
						cout << " |" << endl;
					}
				}
				break;
			}
			}
		}
		for (int i = 0; i < order; i++) {
			printTree(subtree->children[i]);
		}
	}
}

template<class Type>
void BTree<Type>::emptyTree(Node<Type>* subtree) {
	if (subtree != nullptr) {
		for (int i = 0; i < subtree->order; i++) {
			emptyTree(subtree->children[i]);
		}
		delete subtree;
	}
}

template<class Type>
BTree<Type>::~BTree() {
	emptyTree();
}